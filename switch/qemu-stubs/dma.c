/*
 * QEMU Stubs for Nintendo Switch - DMA Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "exec/memory.h"
#include "sysemu/dma.h"

/* Forward declarations */
typedef struct QEMUSGList QEMUSGList;

/*
 * Scatter-gather list functions - only stubbed when NOT building QEMU core.
 * When SWITCH_QEMU_CORE is defined, system/dma-helpers.c provides the real implementations.
 */
#ifndef SWITCH_QEMU_CORE

void qemu_sglist_init(QEMUSGList *qsg, DeviceState *dev, int alloc_hint, AddressSpace *as)
{
    (void)qsg; (void)dev; (void)alloc_hint; (void)as;
}

void qemu_sglist_add(QEMUSGList *qsg, dma_addr_t base, dma_addr_t len)
{
    (void)qsg; (void)base; (void)len;
}

void qemu_sglist_destroy(QEMUSGList *qsg)
{
    (void)qsg;
}

/*
 * Address space functions - only stubbed when NOT building QEMU core.
 * When SWITCH_QEMU_CORE is defined, system/physmem.c provides the real implementation.
 */
MemTxResult address_space_rw(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, void *buf, hwaddr len, bool is_write)
{
    (void)as; (void)addr; (void)attrs; (void)buf; (void)len; (void)is_write;
    return MEMTX_OK;
}

#endif /* !SWITCH_QEMU_CORE */