/*
 * QEMU aio implementation - Nintendo Switch
 *
 * Based on aio-win32.c, adapted for Switch's LEvent-based EventNotifiers.
 * Switch does not have POSIX file descriptor semantics; this implementation
 * uses libnx LEvent for all synchronization.
 *
 * Copyright IBM Corp., 2008
 * Copyright Red Hat Inc., 2012
 * Copyright (c) 2024-2026 xemu contributors
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "config-switch.h"

/* libnx headers FIRST to own the namespace */
#define ThreadContext LibnxThreadContext
#include <switch/kernel/levent.h>
#undef ThreadContext
#undef BIT

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* QEMU headers - minimal set */
#include "qemu/osdep.h"
#include "block/aio.h"
#include "block/aio-wait.h"
#include "qemu/main-loop.h"
#include "qemu/lockcnt.h"
#include "qemu/queue.h"
#include "qapi/error.h"
#include "qemu/rcu_queue.h"
#include "qemu/error-report.h"

/*
 * AioHandler for Switch - stores EventNotifier pointer directly
 * (no file descriptor, unlike POSIX)
 */
struct AioHandler {
    EventNotifier *e;
    EventNotifierHandler *io_notify;
    int deleted;
    bool ready;
    void *opaque;
    QLIST_ENTRY(AioHandler) node;
};

static void aio_remove_fd_handler(AioContext *ctx, AioHandler *node)
{
    /*
     * If the GSource is in the process of being destroyed then
     * g_source_remove_poll() causes an assertion failure. Skip
     * removal in that case.
     */
    if (!g_source_is_destroyed(&ctx->source)) {
        /* No GPollFD on Switch - nothing to remove from poll */
    }

    /* If aio_poll is in progress, just mark the node as deleted */
    if (qemu_lockcnt_count(&ctx->list_lock)) {
        node->deleted = 1;
    } else {
        /* Otherwise, delete it for real */
        QLIST_REMOVE(node, node);
        g_free(node);
    }
}

/*
 * aio_set_fd_handler - Not supported on Switch
 *
 * Switch does not have POSIX file descriptors. This function logs an error
 * and does nothing. xemu does not require fd-based I/O for its core
 * functionality.
 */
void aio_set_fd_handler(AioContext *ctx, int fd, IOHandler *io_read,
                        IOHandler *io_write, AioPollFn *io_poll,
                        IOHandler *io_poll_ready, void *opaque)
{
    /* Log error only when trying to add a handler, not when removing */
    if (io_read || io_write) {
        error_report("aio_set_fd_handler: fd=%d not supported on Switch", fd);
    }
}

/*
 * aio_set_event_notifier - Register an EventNotifier with the AioContext
 */
void aio_set_event_notifier(AioContext *ctx, EventNotifier *e,
                            EventNotifierHandler *io_notify, AioPollFn *io_poll,
                            EventNotifierHandler *io_poll_ready)
{
    AioHandler *node;

    qemu_lockcnt_lock(&ctx->list_lock);
    QLIST_FOREACH (node, &ctx->aio_handlers, node) {
        if (node->e == e && !node->deleted) {
            break;
        }
    }

    /* Are we deleting the handler? */
    if (!io_notify) {
        if (node) {
            aio_remove_fd_handler(ctx, node);
        }
    } else {
        if (node == NULL) {
            /* Alloc and insert if it's not already there */
            node = g_new0(AioHandler, 1);
            node->e = e;
            QLIST_INSERT_HEAD_RCU(&ctx->aio_handlers, node, node);
        }
        /* Update handler with latest information */
        node->io_notify = io_notify;
        node->ready = false;
    }

    qemu_lockcnt_unlock(&ctx->list_lock);
    aio_notify(ctx);
}

/*
 * aio_set_event_notifier_poll - Not implemented
 *
 * Polling mode optimization not supported on Switch.
 */
void aio_set_event_notifier_poll(AioContext *ctx, EventNotifier *notifier,
                                 EventNotifierHandler *io_poll_begin,
                                 EventNotifierHandler *io_poll_end)
{
    /* Not implemented - polling optimization not needed */
}

/*
 * aio_prepare - Check for ready handlers (GSource callback)
 */
bool aio_prepare(AioContext *ctx)
{
    AioHandler *node;
    bool have_ready = false;

    qemu_lockcnt_inc(&ctx->list_lock);

    QLIST_FOREACH_RCU (node, &ctx->aio_handlers, node) {
        if (!node->deleted && node->e && node->io_notify) {
            if (node->ready) {
                have_ready = true;
                continue;
            }

            /* Consume and latch readiness for dispatch. */
            if (node->e->initialized && leventTryWait(node->e->levent)) {
                node->ready = true;
                have_ready = true;
            }
        }
    }

    qemu_lockcnt_dec(&ctx->list_lock);
    return have_ready;
}

/*
 * aio_pending - Check if any handlers have pending events
 */
bool aio_pending(AioContext *ctx)
{
    AioHandler *node;
    bool result = false;

    qemu_lockcnt_inc(&ctx->list_lock);

    QLIST_FOREACH_RCU (node, &ctx->aio_handlers, node) {
        if (!node->deleted && node->e && node->e->initialized &&
            node->io_notify) {
            if (node->ready || leventTryWait(node->e->levent)) {
                node->ready = true;
                result = true;
                break;
            }
        }
    }

    qemu_lockcnt_dec(&ctx->list_lock);
    return result;
}

/*
 * aio_dispatch_handlers - Dispatch handlers that are ready
 *
 * Returns true if any progress was made.
 */
static bool aio_dispatch_handlers(AioContext *ctx)
{
    AioHandler *node;
    AioHandler *tmp;
    bool progress = false;

    QLIST_FOREACH_SAFE_RCU (node, &ctx->aio_handlers, node, tmp) {
        if (!node->deleted && node->e && node->e->initialized &&
            node->io_notify) {
            if (node->ready || leventTryWait(node->e->levent)) {
                node->ready = false;
                node->io_notify(node->e);

                /* aio_notify() does not count as progress */
                if (node->e != &ctx->notifier) {
                    progress = true;
                }
            }
        }

        if (node->deleted) {
            if (qemu_lockcnt_dec_if_lock(&ctx->list_lock)) {
                QLIST_REMOVE(node, node);
                g_free(node);
                qemu_lockcnt_inc_and_unlock(&ctx->list_lock);
            }
        }
    }

    return progress;
}

/*
 * aio_dispatch - Dispatch pending callbacks
 */
void aio_dispatch(AioContext *ctx)
{
    qemu_lockcnt_inc(&ctx->list_lock);
    aio_bh_poll(ctx);
    aio_dispatch_handlers(ctx);
    qemu_lockcnt_dec(&ctx->list_lock);
    timerlistgroup_run_timers(&ctx->tlg);
}

/*
 * aio_poll - Main event loop
 *
 * Wait for events to occur and dispatch handlers.
 * Uses leventWait() on the context notifier for blocking waits,
 * then polls all registered EventNotifiers.
 */
bool aio_poll(AioContext *ctx, bool blocking)
{
    bool progress;
    int64_t timeout;

    /*
     * There cannot be two concurrent aio_poll calls for the same AioContext.
     */
    assert(in_aio_context_home_thread(
        ctx == iohandler_get_aio_context() ? qemu_get_aio_context() : ctx));
    progress = false;

    /*
     * aio_notify can avoid the expensive event_notifier_set if
     * everything will be re-evaluated before the next blocking poll.
     */
    if (blocking) {
        qatomic_set(&ctx->notify_me, qatomic_read(&ctx->notify_me) + 2);
        smp_mb();
    }

    qemu_lockcnt_inc(&ctx->list_lock);

    /* First, process any pending bottom halves */
    progress = aio_bh_poll(ctx);

    /* Calculate timeout */
    timeout = blocking ? aio_compute_timeout(ctx) : 0;

    /* If blocking and we have no immediate work, wait on the notifier */
    if (timeout > 0 && !progress) {
        uint64_t timeout_ns = (uint64_t)timeout;

        /* Wait on the context's notifier LEvent */
        if (ctx->notifier.initialized && ctx->notifier.levent) {
            leventWait(ctx->notifier.levent, timeout_ns);
        } else {
            /* Fallback: short sleep if notifier not available */
            svcSleepThread(timeout_ns > 1000000 ? 1000000 : timeout_ns);
        }
    }

    if (blocking) {
        qatomic_store_release(&ctx->notify_me,
                              qatomic_read(&ctx->notify_me) - 2);
        aio_notify_accept(ctx);
    }

    /* Dispatch all ready handlers */
    progress |= aio_dispatch_handlers(ctx);

    qemu_lockcnt_dec(&ctx->list_lock);

    progress |= timerlistgroup_run_timers(&ctx->tlg);
    return progress;
}

/*
 * aio_context_setup - Initialize the AioContext
 */
void aio_context_setup(AioContext *ctx)
{
    /* No epoll or io_uring on Switch */
    ctx->epollfd = -1;
    ctx->fdmon_ops = NULL;
}

/*
 * aio_context_destroy - Destroy the AioContext
 */
void aio_context_destroy(AioContext *ctx)
{
    /* Clean up any remaining handlers */
    AioHandler *node;
    AioHandler *tmp;

    QLIST_FOREACH_SAFE (node, &ctx->aio_handlers, node, tmp) {
        QLIST_REMOVE(node, node);
        g_free(node);
    }
}

/*
 * aio_context_use_g_source - Prepare for GSource usage
 */
void aio_context_use_g_source(AioContext *ctx)
{
    /* Nothing to do on Switch */
}

/*
 * aio_context_set_poll_params - Configure polling parameters
 *
 * Polling mode is not supported on Switch.
 */
void aio_context_set_poll_params(AioContext *ctx, int64_t max_ns, int64_t grow,
                                 int64_t shrink, Error **errp)
{
    if (max_ns) {
        error_setg(errp, "AioContext polling is not implemented on Switch");
    }
}

/*
 * aio_context_set_aio_params - Configure AIO engine parameters
 */
void aio_context_set_aio_params(AioContext *ctx, int64_t max_batch)
{
    /* Nothing to do */
}

/*
 * aio_poll_disabled - Check if polling is disabled
 *
 * Always returns true on Switch (no userspace polling).
 */
bool aio_poll_disabled(AioContext *ctx)
{
    return true;
}
