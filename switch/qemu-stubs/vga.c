/*
 * QEMU Stubs for Nintendo Switch - VGA
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * VGA helpers (unused on Switch)
 */
typedef struct VGACommonState VGACommonState;

__attribute__((weak)) uint32_t vga_ioport_read(void *opaque, uint32_t addr)
{
    (void)opaque;
    (void)addr;
    return 0;
}

__attribute__((weak)) void vga_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    (void)opaque;
    (void)addr;
    (void)val;
}

__attribute__((weak)) void vga_invalidate_scanlines(VGACommonState *s, int y1, int y2)
{
    (void)s;
    (void)y1;
    (void)y2;
}

__attribute__((weak)) void graphic_hw_update(void *con)
{
    (void)con;
}

__attribute__((weak)) const MemoryRegionOps serial_io_ops = { 0 };
