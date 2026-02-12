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
static uint32_t switch_pstraps0_primary   = 0x000c4000;
static uint32_t switch_pstraps0_select    = 0x00000000;
static uint32_t switch_pstraps0_secondary = 0x000c4000;
static uint32_t switch_pstraps1_primary   = 0x00000000;
static uint32_t switch_pstraps1_select    = 0x00000000;
static uint32_t switch_pstraps1_secondary = 0x00000000;
#endif

#define DEFINE_STUB(name, region_id) \
    uint64_t name ## _read(void *opaque, \
                           hwaddr addr, \
                           unsigned int size) \
    { \
        nv2a_reg_log_read(region_id, addr, size, 0); \
        return 0; \
    } \
    void name ## _write(void *opaque, \
                        hwaddr addr, \
                        uint64_t val, \
                        unsigned int size) \
    { \
        nv2a_reg_log_write(region_id, addr, size, val); \
    } \

DEFINE_STUB(prma, NV_PRMA)
DEFINE_STUB(pcounter, NV_PCOUNTER)
DEFINE_STUB(pvpe, NV_PVPE)
DEFINE_STUB(ptv, NV_PTV)
DEFINE_STUB(prmfb, NV_PRMFB)
// DEFINE_STUB(pramin, NV_PRAMIN)

#undef DEFINE_STUB

uint64_t pstraps_read(void *opaque, hwaddr addr, unsigned int size)
{
    uint64_t r = 0;
#ifdef CONFIG_SWITCH
    /*
     * Keep desktop behavior unchanged (still mostly unimplemented), but provide
     * a minimally plausible strap image on Switch so BIOS strap branches are
     * not forced down the all-zero path.
     */
    static unsigned pstraps_read_log_count;

    switch (addr) {
    case 0x000:
        r = switch_pstraps0_primary;
        break;
    case 0x004:
        r = switch_pstraps0_select;
        break;
    case 0x008:
        r = switch_pstraps0_secondary;
        break;
    case 0x00c:
        r = switch_pstraps1_primary;
        break;
    case 0x010:
        r = switch_pstraps1_select;
        break;
    case 0x014:
        r = switch_pstraps1_secondary;
        break;
    default:
        r = 0;
        break;
    }

    if (pstraps_read_log_count < 300 ||
        (pstraps_read_log_count % 2000) == 0) {
        fprintf(stderr,
                "Switch: pstraps read addr=0x%03" HWADDR_PRIx
                " val=0x%08" PRIx64 "\n",
                addr, r);
    }
    pstraps_read_log_count++;
#endif

    nv2a_reg_log_read(NV_PSTRAPS, addr, size, r);
    return r;
}

void pstraps_write(void *opaque, hwaddr addr, uint64_t val, unsigned int size)
{
    (void)opaque;
#ifdef CONFIG_SWITCH
    static unsigned pstraps_write_log_count;

    switch (addr) {
    case 0x000:
        switch_pstraps0_primary = val;
        break;
    case 0x004:
        switch_pstraps0_select = val;
        break;
    case 0x008:
        switch_pstraps0_secondary = val;
        break;
    case 0x00c:
        switch_pstraps1_primary = val;
        break;
    case 0x010:
        switch_pstraps1_select = val;
        break;
    case 0x014:
        switch_pstraps1_secondary = val;
        break;
    default:
        break;
    }

    if (pstraps_write_log_count < 300 ||
        (pstraps_write_log_count % 2000) == 0) {
        fprintf(stderr,
                "Switch: pstraps write addr=0x%03" HWADDR_PRIx
                " val=0x%08" PRIx64 "\n",
                addr, val);
    }
    pstraps_write_log_count++;
#else
    (void)addr;
    (void)val;
    (void)size;
#endif

    nv2a_reg_log_write(NV_PSTRAPS, addr, size, val);
}
