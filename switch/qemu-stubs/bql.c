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

void qemu_mutex_unlock_main_loop(void)
{
}

void qemu_init_main_loop_lock(void)
{
}
