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

/* Scatter-gather list functions */
void qemu_sglist_init(QEMUSGList *qsg, DeviceState *dev, int alloc_hint, AddressSpace *as)
{
    (void)qsg; (void)dev; (void)alloc_hint; (void)as;
}

void qemu_sglist_add(QEMUSGList *qsg, dma_addr_t base, dma_addr_t len)
{
    (void)qsg; (void)base; (void)len;
}

/* Address space functions */
MemTxResult address_space_rw(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, void *buf, hwaddr len, bool is_write)
{
    (void)as; (void)addr; (void)attrs; (void)buf; (void)len; (void)is_write;
    return MEMTX_OK;
}

void qemu_sglist_destroy(QEMUSGList *qsg)
{
    (void)qsg;
}