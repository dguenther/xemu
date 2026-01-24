/*
 * QEMU Stubs for Nintendo Switch - Utility Functions  
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void qemu_hexdump_line(char *buf, size_t len, const void *mem, size_t mem_size, unsigned int unit_size, unsigned int units_per_line)
{
    (void)buf; (void)len; (void)mem; (void)mem_size; (void)unit_size; (void)units_per_line;
}

void qemu_notify_event(void)
{
    /* No-op on Switch */
}

__attribute__((weak)) ICountMode use_icount = 0;

char *qemu_find_file(int type, const char *name)
{
    (void)type; (void)name;
    return NULL;
}

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

GSource *g_source_ref(GSource *source)
{
    return source;
}

void g_source_unref(GSource *source)
{
    (void)source;
}
