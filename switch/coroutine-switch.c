/*
 * ARM64/Switch coroutine backend
 *
 * Based on util/coroutine-ucontext.c but adapted for Nintendo Switch
 * which lacks makecontext/swapcontext. Uses ARM64 inline assembly
 * for initial stack setup and sigsetjmp/siglongjmp for context switching.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/* Disable glibc's stack check for longjmp */
#undef _FORTIFY_SOURCE
#define _FORTIFY_SOURCE 0

#include "qemu/osdep.h"
#include <setjmp.h>
#include "qemu/coroutine_int.h"
#include "qemu/coroutine-tls.h"

typedef struct {
    Coroutine base;
    void *stack;
    size_t stack_size;
    sigjmp_buf env;
} CoroutineSwitch;

/**
 * Per-thread coroutine bookkeeping
 */
QEMU_DEFINE_STATIC_CO_TLS(Coroutine *, current);
QEMU_DEFINE_STATIC_CO_TLS(CoroutineSwitch, leader);

/*
 * Coroutine trampoline function - runs when a new coroutine is first entered
 */
static void coroutine_trampoline(CoroutineSwitch *self)
{
    Coroutine *co = &self->base;

    /* Initialize longjmp environment and switch back to caller */
    if (!sigsetjmp(self->env, 0)) {
        /* Jump back to qemu_coroutine_new() to complete initialization */
        siglongjmp(*(sigjmp_buf *)co->entry_arg, 1);
    }

    /* When we return here via siglongjmp, run the coroutine entry function */
    while (true) {
        co->entry(co->entry_arg);
        qemu_coroutine_switch(co, co->caller, COROUTINE_TERMINATE);
    }
}

/*
 * ARM64 assembly to set up initial coroutine stack and call trampoline
 *
 * This is the ARM64 equivalent of makecontext(). We need to:
 * 1. Switch to the new stack
 * 2. Call the trampoline function
 * 3. The trampoline will save its context and return to us
 */
static void start_coroutine(CoroutineSwitch *co)
{
    void *stack_top = co->stack + co->stack_size;

    /* ARM64 stack must be 16-byte aligned */
    stack_top = (void *)((uintptr_t)stack_top & ~15UL);

    /*
     * Switch to new stack and call trampoline.
     * ARM64 calling convention:
     * - x0 = first argument (co)
     * - sp = stack pointer
     * - x30 (lr) = return address
     */
    __asm__ __volatile__(
        "mov x9, sp\n\t"              /* Save old stack pointer in x9 */
        "mov sp, %[new_sp]\n\t"       /* Switch to new stack */
        "mov x0, %[co]\n\t"           /* First arg: co */
        "blr %[trampoline]\n\t"       /* Call trampoline */
        "mov sp, x9\n\t"              /* Restore old stack pointer */
        :
        : [new_sp] "r" (stack_top),
          [co] "r" (co),
          [trampoline] "r" (coroutine_trampoline)
        : "x0", "x9", "x30", "memory"
    );
}

Coroutine *qemu_coroutine_new(void)
{
    CoroutineSwitch *co;
    sigjmp_buf old_env;

    co = g_malloc0(sizeof(*co));
    co->stack_size = COROUTINE_STACK_SIZE;
    co->stack = g_malloc(co->stack_size);

    if (!co->stack) {
        g_free(co);
        return NULL;
    }

    /* Stash the old_env pointer so trampoline can return to us */
    co->base.entry_arg = &old_env;

    /* Set up the new coroutine's stack and initialize its context */
    if (!sigsetjmp(old_env, 0)) {
        start_coroutine(co);
    }

    /* When we return here, the trampoline has saved its context */
    return &co->base;
}

void qemu_coroutine_delete(Coroutine *co_)
{
    CoroutineSwitch *co = (CoroutineSwitch *)co_;

    if (co->stack) {
        g_free(co->stack);
    }
    g_free(co);
}

/*
 * Switch from one coroutine to another
 * This is marked noinline to prevent GCC from hoisting TLS access
 * out of the loop in coroutine_trampoline().
 */
CoroutineAction __attribute__((noinline))
qemu_coroutine_switch(Coroutine *from_, Coroutine *to_,
                      CoroutineAction action)
{
    CoroutineSwitch *from = (CoroutineSwitch *)from_;
    CoroutineSwitch *to = (CoroutineSwitch *)to_;
    int ret;

    set_current(to_);

    ret = sigsetjmp(from->env, 0);
    if (ret == 0) {
        siglongjmp(to->env, action);
    }

    return ret;
}

Coroutine *qemu_coroutine_self(void)
{
    Coroutine *self = get_current();
    CoroutineSwitch *leaderp = get_ptr_leader();

    if (!self) {
        self = &leaderp->base;
        set_current(self);
    }
    return self;
}

bool qemu_in_coroutine(void)
{
    Coroutine *self = get_current();

    return self && self->caller;
}
