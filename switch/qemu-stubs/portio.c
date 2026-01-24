/*
 * QEMU Stubs for Nintendo Switch - Port I/O Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"

/* Forward declarations */
typedef struct PortioList PortioList;
typedef struct MemoryRegionPortio MemoryRegionPortio;

/* Port I/O list functions */
void portio_list_init(PortioList *piolist, Object *owner, const MemoryRegionPortio *callbacks, void *opaque, const char *name)
{
    (void)piolist; (void)owner; (void)callbacks; (void)opaque; (void)name;
}

void portio_list_add(PortioList *piolist, MemoryRegion *address_space, uint32_t addr)
{
    (void)piolist; (void)address_space; (void)addr;
}