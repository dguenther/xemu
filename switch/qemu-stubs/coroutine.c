/*
 * QEMU Stubs for Nintendo Switch - Coroutines and AIO
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"
#include "qemu/coroutine.h"

/*
 * AIO stubs (synchronous fallback)
 * Coroutine implementation now provided by switch/coroutine-switch.c
 */
typedef struct AioContext AioContext;
typedef struct Coroutine Coroutine;
typedef struct {
    unsigned num_waiters;
} AioWait;

AioWait global_aio_wait = { 0 };

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

void main_loop_wait(int nonblocking)
{
    (void)nonblocking;
}
