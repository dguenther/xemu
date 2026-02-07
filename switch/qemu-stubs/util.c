/*
 * QEMU Stubs for Nintendo Switch - Utility Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void qemu_notify_event(void)
{
    /* No-op on Switch */
}

__attribute__((weak)) ICountMode use_icount = 0;

void defer_call_begin(void)
{
}

void defer_call_end(void)
{
}

void *iohandler_get_aio_context(void)
{
    return NULL;
}
