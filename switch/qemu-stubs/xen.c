/*
 * QEMU Stubs for Nintendo Switch - Xen helpers
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

__attribute__((weak)) void xen_register_framebuffer(MemoryRegion *mr)
{
    (void)mr;
}
