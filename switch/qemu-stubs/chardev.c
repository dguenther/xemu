/*
 * QEMU Stubs for Nintendo Switch - Chardev (Character Devices)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/* Forward declarations */
typedef struct Chardev Chardev;

int qmp_chardev_open_file_source(const char *path, int flags, Error **errp)
{
    (void)path;
    (void)flags;
    (void)errp;
    return -1;
}

void *qemu_chr_open_fd(int fd_in, int fd_out)
{
    (void)fd_in;
    (void)fd_out;
    return NULL;
}

void *qemu_main_context = NULL;

/* Stub for g_source_destroy - needed by chardev but not in glib-compat */
void g_source_destroy(GSource *source)
{
    (void)source;
    /* No-op stub */
}

/*
 * serial_hd is only stubbed when NOT building the QEMU core.
 * When SWITCH_QEMU_CORE is defined, system/vl.c provides the real implementation.
 */
#ifndef SWITCH_QEMU_CORE
Chardev *serial_hd(int i)
{
    (void)i;
    return NULL;
}
#endif /* !SWITCH_QEMU_CORE */
