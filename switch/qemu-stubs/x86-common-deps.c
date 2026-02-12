/*
 * QEMU Stubs for Nintendo Switch - x86-common dependencies
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/i386/x86.h"
#include "hw/i386/apic.h"
#include "sysemu/numa.h"

__attribute__((weak))
void init_topo_info(X86CPUTopoInfo *topo_info, const X86MachineState *x86ms)
{
    fprintf(stderr, "STUB: init_topo_info called\n");
    const MachineState *ms = MACHINE(x86ms);

    topo_info->dies_per_pkg = ms->smp.dies ? ms->smp.dies : 1;
    topo_info->modules_per_die = ms->smp.modules ? ms->smp.modules : 1;
    topo_info->cores_per_module = ms->smp.cores ? ms->smp.cores : 1;
    topo_info->threads_per_core = ms->smp.threads ? ms->smp.threads : 1;
}

__attribute__((weak))
uint32_t x86_cpu_apic_id_from_index(X86MachineState *x86ms,
                                    unsigned int cpu_index)
{
    fprintf(stderr, "STUB: x86_cpu_apic_id_from_index called\n");
    X86CPUTopoInfo topo_info;

    init_topo_info(&topo_info, x86ms);
    return x86_apicid_from_cpu_idx(&topo_info, cpu_index);
}

__attribute__((weak))
void apic_set_max_apic_id(uint32_t max_apic_id)
{
    fprintf(stderr, "STUB: apic_set_max_apic_id called\n");
    (void)max_apic_id;
}

__attribute__((weak))
void numa_cpu_pre_plug(const struct CPUArchId *slot, DeviceState *dev,
                       Error **errp)
{
    fprintf(stderr, "STUB: numa_cpu_pre_plug called\n");
    (void)slot;
    (void)dev;
    (void)errp;
}
