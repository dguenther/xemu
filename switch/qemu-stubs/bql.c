/*
 * QEMU Stubs for Nintendo Switch - Big QEMU Lock (BQL)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Big QEMU Lock (BQL) stubs
 * Note: bql_lock_impl, bql_unlock, and bql_locked are now provided by system/cpus.c
 * We only need to provide the main loop lock functions that may not be in all builds
 */

void qemu_mutex_lock_main_loop(void)
{
}

void qemu_init_main_loop_lock(void)
{
}

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
