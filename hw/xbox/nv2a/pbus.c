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
#define NV_PBUS_PROBE_DATA  0x0000021c
#define NV_PBUS_PROBE_CTRL0 0x00000220
#define NV_PBUS_PROBE_CTRL1 0x00000228
#define NV_PBUS_SHADOW_BASE 0x00000200
#define NV_PBUS_SHADOW_SIZE 0x00000100

static inline hwaddr pbus_reg_offset(hwaddr addr)
{
    /*
     * PBUS callbacks usually receive block-relative offsets (0x000..0xfff).
     * Keep compatibility with absolute-style offsets if they appear.
     */
    if (addr >= 0x00001000 && addr < 0x00002000) {
        return addr - 0x00001000;
    }
    return addr;
}

static inline bool pbus_is_shadow_reg(hwaddr reg)
{
    if (reg < NV_PBUS_SHADOW_BASE ||
        reg >= (NV_PBUS_SHADOW_BASE + NV_PBUS_SHADOW_SIZE)) {
        return false;
    }
    return true;
}

static inline uint32_t pbus_shadow_get_dword(NV2AState *s, hwaddr reg)
{
    hwaddr aligned = reg & ~0x3;
    assert(pbus_is_shadow_reg(aligned));
    return s->pbus.shadow_regs[(aligned - NV_PBUS_SHADOW_BASE) >> 2];
}

static inline void pbus_shadow_set_dword(NV2AState *s, hwaddr reg, uint32_t val)
{
    hwaddr aligned = reg & ~0x3;
    assert(pbus_is_shadow_reg(aligned));
    s->pbus.shadow_regs[(aligned - NV_PBUS_SHADOW_BASE) >> 2] = val;
}

static inline uint64_t pbus_shadow_read(NV2AState *s, hwaddr reg,
                                        unsigned int size)
{
    uint64_t val = 0;
    unsigned int i;

    for (i = 0; i < size; i++) {
        hwaddr cur = reg + i;
        uint32_t dword = pbus_shadow_get_dword(s, cur);
        unsigned shift = (unsigned)((cur & 0x3) * 8);
        val |= (uint64_t)((dword >> shift) & 0xff) << (i * 8);
    }

    return val;
}

static inline void pbus_shadow_write(NV2AState *s, hwaddr reg, uint64_t val,
                                     unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++) {
        hwaddr cur = reg + i;
        uint32_t dword = pbus_shadow_get_dword(s, cur);
        unsigned shift = (unsigned)((cur & 0x3) * 8);
        uint32_t mask = (uint32_t)0xff << shift;
        uint32_t byte = (uint32_t)((val >> (i * 8)) & 0xff);

        dword = (dword & ~mask) | (byte << shift);
        pbus_shadow_set_dword(s, cur, dword);
    }
}
#endif

#ifdef CONFIG_SWITCH
static uint32_t pbus_probe_next(NV2AState *s)
{
    /*
     * On hardware this appears to return changing values tied to PBUS probe
     * control writes. Returning a deterministic evolving value avoids getting
     * stuck in BIOS probe loops that expect non-constant data.
     */
    s->pbus.lfsr = (s->pbus.lfsr * 1664525u) + 1013904223u +
                   (s->pbus.probe_ctrl0 ^ s->pbus.probe_ctrl1);
    s->pbus.probe_data = s->pbus.lfsr ^ (s->pbus.probe_ctrl1 << 7);
    return s->pbus.probe_data;
}
#endif

/* PBUS - bus control */
uint64_t pbus_read(void *opaque, hwaddr addr, unsigned int size)
{
    NV2AState *s = opaque;
    PCIDevice *d = PCI_DEVICE(s);
    hwaddr reg = addr;
#ifdef CONFIG_SWITCH
    reg = pbus_reg_offset(addr);
#endif

    uint64_t r = 0;
    switch (reg) {
    case NV_PBUS_PCI_NV_0:
        r = pci_get_long(d->config + PCI_VENDOR_ID);
        break;
    case NV_PBUS_PCI_NV_1:
        r = pci_get_long(d->config + PCI_COMMAND);
        break;
    case NV_PBUS_PCI_NV_2:
        r = pci_get_long(d->config + PCI_CLASS_REVISION);
        break;
#ifdef CONFIG_SWITCH
    case NV_PBUS_PROBE_DATA:
        r = pbus_probe_next(s);
        pbus_shadow_set_dword(s, reg, r);
        break;
    case NV_PBUS_PROBE_CTRL0:
        r = s->pbus.probe_ctrl0;
        break;
    case NV_PBUS_PROBE_CTRL1:
        r = s->pbus.probe_ctrl1;
        break;
#endif
    default:
#ifdef CONFIG_SWITCH
        if (pbus_is_shadow_reg(reg)) {
            r = pbus_shadow_read(s, reg, size);
        }
#endif
        break;
    }

#ifdef CONFIG_SWITCH
    static unsigned pbus_probe_read_log_count;
    bool log_read_reg =
        reg == NV_PBUS_PROBE_DATA ||
        reg == NV_PBUS_PROBE_CTRL0 ||
        reg == NV_PBUS_PROBE_CTRL1 ||
        pbus_is_shadow_reg(reg);
    if (log_read_reg &&
        (pbus_probe_read_log_count < 300 ||
         (pbus_probe_read_log_count % 2000) == 0)) {
        fprintf(stderr,
                "Switch: pbus read addr=0x%04" HWADDR_PRIx
                " reg=0x%04" HWADDR_PRIx
                " val=0x%08" PRIx64 " ctrl0=0x%08x ctrl1=0x%08x\n",
                addr, reg, r, s->pbus.probe_ctrl0, s->pbus.probe_ctrl1);
    }
    if (log_read_reg) {
        pbus_probe_read_log_count++;
    }
#endif

    nv2a_reg_log_read(NV_PBUS, addr, size, r);
    return r;
}

void pbus_write(void *opaque, hwaddr addr, uint64_t val, unsigned int size)
{
    NV2AState *s = opaque;
    PCIDevice *d = PCI_DEVICE(s);
    hwaddr reg = addr;
#ifdef CONFIG_SWITCH
    reg = pbus_reg_offset(addr);
#endif

    nv2a_reg_log_write(NV_PBUS, addr, size, val);

    switch (reg) {
    case NV_PBUS_PCI_NV_1:
        pci_set_long(d->config + PCI_COMMAND, val);
        break;
#ifdef CONFIG_SWITCH
    case NV_PBUS_PROBE_CTRL0:
        pbus_shadow_write(s, reg, val, size);
        s->pbus.probe_ctrl0 = pbus_shadow_get_dword(s, NV_PBUS_PROBE_CTRL0);
        break;
    case NV_PBUS_PROBE_CTRL1:
        pbus_shadow_write(s, reg, val, size);
        s->pbus.probe_ctrl1 = pbus_shadow_get_dword(s, NV_PBUS_PROBE_CTRL1);
        break;
#endif
    default:
#ifdef CONFIG_SWITCH
        if (pbus_is_shadow_reg(reg)) {
            pbus_shadow_write(s, reg, val, size);
        }
#endif
        break;
    }

#ifdef CONFIG_SWITCH
    static unsigned pbus_probe_write_log_count;
    bool log_write_reg =
        reg == NV_PBUS_PROBE_CTRL0 ||
        reg == NV_PBUS_PROBE_CTRL1 ||
        pbus_is_shadow_reg(reg);
    if (log_write_reg &&
        (pbus_probe_write_log_count < 300 ||
         (pbus_probe_write_log_count % 2000) == 0)) {
        fprintf(stderr,
                "Switch: pbus write addr=0x%04" HWADDR_PRIx
                " reg=0x%04" HWADDR_PRIx
                " val=0x%08" PRIx64 " ctrl0=0x%08x ctrl1=0x%08x\n",
                addr, reg, val, s->pbus.probe_ctrl0, s->pbus.probe_ctrl1);
    }
    if (log_write_reg) {
        pbus_probe_write_log_count++;
    }
#endif
}
