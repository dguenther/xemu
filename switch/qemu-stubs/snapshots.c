/*
 * QEMU Stubs for Nintendo Switch - GDB Server
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void xemu_snapshots_offset_extra_data(void *extra_data, size_t extra_data_size)
{
    switch_log("STUB: xemu_snapshots_offset_extra_data called\n");
    (void)extra_data;
    (void)extra_data_size;
}

void xemu_snapshots_save_extra_data(void *extra_data, size_t extra_data_size)
{
    switch_log("STUB: xemu_snapshots_save_extra_data called\n");
    (void)extra_data;
    (void)extra_data_size;
}
