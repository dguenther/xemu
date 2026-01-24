/*
 * QEMU Stubs for Nintendo Switch - Big QEMU Lock (BQL)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * These functions are called by system/runstate.c but are normally provided
 * by util/main-loop.c which we don't include. Always provide stubs.
 */
void qemu_mutex_lock_main_loop(void)
{
}

void qemu_init_main_loop_lock(void)
{
}

/*
 * The following BQL functions are only stubbed when NOT building the QEMU core.
 * When SWITCH_QEMU_CORE is defined, system/cpus.c provides the real implementations.
 */
#ifndef SWITCH_QEMU_CORE

/* Additional BQL functions */
void qemu_cond_wait_bql(QemuCond *cond)
{
    (void)cond;
}

void qemu_wait_io_event_common(CPUState *cpu)
{
    (void)cpu;
}

void qemu_wait_io_event(void)
{
}

/* BQL locked check */
bool bql_locked(void)
{
    return true;
}

/* BQL lock/unlock implementations */
void bql_lock_impl(void)
{
}

void bql_unlock(void)
{
}

#endif /* !SWITCH_QEMU_CORE */
