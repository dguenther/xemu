/*
 * QEMU Stubs for Nintendo Switch - fw_cfg helpers
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

__attribute__((weak))
void fw_cfg_add_file_callback(FWCfgState *s, const char *filename,
                              void (*select_cb)(void *opaque),
                              void (*write_cb)(void *opaque, off_t start,
                                               size_t len),
                              void *callback_opaque,
                              void *data, size_t len, bool read_only)
{
    (void)s;
    (void)filename;
    (void)select_cb;
    (void)write_cb;
    (void)callback_opaque;
    (void)data;
    (void)len;
    (void)read_only;
}

__attribute__((weak))
void *fw_cfg_modify_file(FWCfgState *s, const char *filename, void *data,
                         size_t len)
{
    (void)s;
    (void)filename;
    (void)data;
    (void)len;
    return NULL;
}
