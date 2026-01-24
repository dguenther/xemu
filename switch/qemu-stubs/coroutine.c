/*
 * QEMU Stubs for Nintendo Switch - Coroutines and AIO
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Coroutine/AIO stubs (synchronous fallback)
 */
typedef struct AioContext AioContext;
typedef struct {
    unsigned num_waiters;
} AioWait;

AioWait global_aio_wait = { 0 };
typedef void CoroutineEntry(void *opaque);
typedef struct Coroutine {
    CoroutineEntry *entry;
    void *opaque;
    bool entered;
} Coroutine;

static Coroutine switch_main_coroutine = { 0 };

__attribute__((weak)) Coroutine *qemu_coroutine_create(CoroutineEntry *entry, void *opaque)
{
    Coroutine *co = calloc(1, sizeof(*co));
    if (!co) {
        return NULL;
    }
    co->entry = entry;
    co->opaque = opaque;
    return co;
}

__attribute__((weak)) Coroutine *qemu_coroutine_self(void)
{
    return &switch_main_coroutine;
}

__attribute__((weak)) void qemu_coroutine_enter(Coroutine *co)
{
    if (!co || !co->entry) {
        return;
    }
    if (co->entered) {
        return;
    }
    co->entered = true;
    co->entry(co->opaque);
    co->entered = false;
}

__attribute__((weak)) void qemu_coroutine_enter_if_inactive(Coroutine *co)
{
    if (!co || co->entered) {
        return;
    }
    qemu_coroutine_enter(co);
}

__attribute__((weak)) void qemu_coroutine_yield(void)
{
    /* No-op: synchronous stub */
}

__attribute__((weak)) bool qemu_coroutine_entered(Coroutine *co)
{
    return co ? co->entered : false;
}

__attribute__((weak)) void qemu_coroutine_delete(Coroutine *co)
{
    if (co && co != &switch_main_coroutine) {
        free(co);
    }
}

__attribute__((weak)) AioContext *qemu_coroutine_get_aio_context(Coroutine *co)
{
    (void)co;
    return NULL;
}

__attribute__((weak)) bool qemu_in_coroutine(void)
{
    return false;
}

__attribute__((weak)) AioContext *qemu_get_aio_context(void)
{
    return NULL;
}

__attribute__((weak)) AioContext *qemu_get_current_aio_context(void)
{
    return NULL;
}

__attribute__((weak)) void aio_wait_kick(void)
{
}

__attribute__((weak)) bool aio_poll(AioContext *ctx, bool blocking)
{
    (void)ctx;
    (void)blocking;
    return false;
}

__attribute__((weak)) void aio_co_enter(AioContext *ctx, Coroutine *co)
{
    (void)ctx;
    qemu_coroutine_enter(co);
}

__attribute__((weak)) void aio_co_wake(Coroutine *co)
{
    (void)co;
}

void qemu_aio_coroutine_enter(AioContext *ctx, Coroutine *co)
{
    (void)ctx;
    qemu_coroutine_enter(co);
}

/*
 * Coroutine mutex stubs (single-threaded fallback)
 */
typedef struct CoMutex CoMutex;

__attribute__((weak)) void qemu_co_mutex_lock(CoMutex *mutex)
{
    (void)mutex;
}

__attribute__((weak)) void qemu_co_mutex_unlock(CoMutex *mutex)
{
    (void)mutex;
}

__attribute__((weak)) void qemu_co_mutex_init(CoMutex *mutex)
{
    (void)mutex;
}

void *qemu_co_sleep_ns_wakeable(void *ns, void *w)
{
    (void)ns;
    (void)w;
    return NULL;
}

void main_loop_wait(int nonblocking)
{
    (void)nonblocking;
}
