/*
 * QEMU Stubs for Nintendo Switch - APIC (x86 Local APIC)
 *
 * Note: These stubs are needed even with the full QEMU core,
 * as certain APIC configuration functions are not implemented on Switch.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

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

/* KVM IRQ tracking stubs */
void kvm_reset_irq_delivered(void)
{
    /* No-op on Switch */
}

int kvm_get_irq_delivered(void)
{
    return 0;
}
