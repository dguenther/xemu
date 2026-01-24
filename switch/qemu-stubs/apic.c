/*
 * QEMU Stubs for Nintendo Switch - APIC (x86 Local APIC)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

#ifdef SWITCH_QEMU_CORE
typedef struct CPUX86State CPUX86State;
typedef struct DeviceState DeviceState;

void cpu_set_apic_base(DeviceState *dev, uint64_t val)
{
    (void)dev;
    (void)val;
}

uint64_t cpu_get_apic_base(DeviceState *dev)
{
    (void)dev;
    return 0xfee00000ULL; /* Default APIC base address */
}

void cpu_set_apic_tpr(DeviceState *dev, uint8_t val)
{
    (void)dev;
    (void)val;
}

uint8_t cpu_get_apic_tpr(DeviceState *dev)
{
    (void)dev;
    return 0;
}

/*
 * CPU address space index stubs
 */
int cpu_asidx_from_attrs(void *cpu, void *attrs)
{
    (void)cpu;
    (void)attrs;
    return 0; /* Default address space index */
}
#endif

int cpu_is_apic_enabled(DeviceState *apic)
{
    (void)apic;
    return 0;
}

int apic_accept_pic_intr(DeviceState *dev)
{
    (void)dev;
    return 1;
}

void apic_deliver_pic_intr(DeviceState *dev, int level)
{
    (void)dev;
    (void)level;
}

void apic_msr_write(void *apic, uint64_t reg, uint64_t val)
{
    (void)apic;
    (void)reg;
    (void)val;
}

uint64_t apic_msr_read(void *apic, uint64_t reg)
{
    (void)apic;
    (void)reg;
    return 0;
}

void apic_init_reset(void *apic)
{
    (void)apic;
}
