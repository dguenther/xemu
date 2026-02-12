/*
 * QEMU Geforce NV2A implementation
 *
 * Copyright (c) 2012 espes
 * Copyright (c) 2015 Jannik Vogel
 * Copyright (c) 2018-2021 Matt Borgerson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "nv2a_int.h"

#ifdef CONFIG_SWITCH
#ifndef SWITCH_PCRTC_DIAG_WRITE_LIMIT
#define SWITCH_PCRTC_DIAG_WRITE_LIMIT 4096
#endif
#endif

uint64_t pcrtc_read(void *opaque, hwaddr addr, unsigned int size)
{
    NV2AState *d = (NV2AState *)opaque;

    uint64_t r = 0;
    switch (addr) {
        case NV_PCRTC_INTR_0:
            r = d->pcrtc.pending_interrupts;
            break;
        case NV_PCRTC_INTR_EN_0:
            r = d->pcrtc.enabled_interrupts;
            break;
        case NV_PCRTC_START:
            r = d->pcrtc.start;
            break;
        case NV_PCRTC_RASTER:
            r = d->pcrtc.raster++;
            break;
        default:
            break;
    }

    nv2a_reg_log_read(NV_PCRTC, addr, size, r);
    return r;
}

void pcrtc_write(void *opaque, hwaddr addr, uint64_t val, unsigned int size)
{
    NV2AState *d = (NV2AState *)opaque;
#ifdef CONFIG_SWITCH
    static unsigned pcrtc_start_log_count;
    static unsigned pcrtc_intr_en_log_count;
    static unsigned pcrtc_write_log_count;
    if (pcrtc_write_log_count < SWITCH_PCRTC_DIAG_WRITE_LIMIT) {
        fprintf(stderr,
                "Switch: pcrtc write addr=0x%03" HWADDR_PRIx
                " val=0x%08" PRIx64 " size=%u idx=%u%s\n",
                addr, val, size, pcrtc_write_log_count,
                (addr == NV_PCRTC_START) ? " start" : "");
    }
    pcrtc_write_log_count++;
#endif

    nv2a_reg_log_write(NV_PCRTC, addr, size, val);

    switch (addr) {
    case NV_PCRTC_INTR_0:
        d->pcrtc.pending_interrupts &= ~val;
        nv2a_update_irq(d);
        break;
    case NV_PCRTC_INTR_EN_0:
        d->pcrtc.enabled_interrupts = val;
        nv2a_update_irq(d);
#ifdef CONFIG_SWITCH
        if (pcrtc_intr_en_log_count < 20 ||
            (pcrtc_intr_en_log_count % 240) == 0) {
            fprintf(stderr,
                    "Switch: pcrtc intr_en=0x%08x pending=0x%08x\n",
                    d->pcrtc.enabled_interrupts,
                    d->pcrtc.pending_interrupts);
        }
        pcrtc_intr_en_log_count++;
#endif
        break;
    case NV_PCRTC_START:
        val &= 0x07FFFFFF;
        // assert(val < memory_region_size(d->vram));
        d->pcrtc.start = val;
#ifdef CONFIG_SWITCH
        if (pcrtc_start_log_count < 20 ||
            (pcrtc_start_log_count % 240) == 0) {
            fprintf(stderr,
                    "Switch: pcrtc start=0x%08x raster=%u\n",
                    (unsigned)d->pcrtc.start, d->pcrtc.raster);
        }
        pcrtc_start_log_count++;
#endif

        NV2A_DPRINTF("PCRTC_START - %x %x %x %x\n",
                d->vram_ptr[val+64], d->vram_ptr[val+64+1],
                d->vram_ptr[val+64+2], d->vram_ptr[val+64+3]);
        break;
    default:
        break;
    }
}
