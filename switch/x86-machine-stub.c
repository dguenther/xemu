/*
 * Minimal x86 machine type registration for Switch bring-up.
 *
 * This avoids pulling in the full hw/i386/x86.c dependency chain while
 * still providing TYPE_X86_MACHINE for TYPE_PC_MACHINE to inherit from.
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/boards.h"
#include "hw/i386/x86.h"
#include "hw/hotplug.h"

static const CPUArchIdList *x86_machine_stub_possible_cpu_arch_ids(MachineState *ms)
{
    X86MachineState *x86ms = X86_MACHINE(ms);
    unsigned int max_cpus = ms->smp.max_cpus;
    X86CPUTopoInfo topo_info;
    int i;

    if (ms->possible_cpus) {
        assert(ms->possible_cpus->len == max_cpus);
        return ms->possible_cpus;
    }

    ms->possible_cpus = g_malloc0(sizeof(CPUArchIdList) +
                                  sizeof(CPUArchId) * max_cpus);
    ms->possible_cpus->len = max_cpus;

    init_topo_info(&topo_info, x86ms);

    for (i = 0; i < ms->possible_cpus->len; i++) {
        X86CPUTopoIDs topo_ids;

        ms->possible_cpus->cpus[i].type = ms->cpu_type;
        ms->possible_cpus->cpus[i].vcpus_count = 1;
        ms->possible_cpus->cpus[i].arch_id =
            x86_cpu_apic_id_from_index(x86ms, i);
        x86_topo_ids_from_apicid(ms->possible_cpus->cpus[i].arch_id,
                                 &topo_info, &topo_ids);
        ms->possible_cpus->cpus[i].props.has_socket_id = true;
        ms->possible_cpus->cpus[i].props.socket_id = topo_ids.pkg_id;
        if (ms->smp.dies > 1) {
            ms->possible_cpus->cpus[i].props.has_die_id = true;
            ms->possible_cpus->cpus[i].props.die_id = topo_ids.die_id;
        }
        if (ms->smp.modules > 1) {
            ms->possible_cpus->cpus[i].props.has_module_id = true;
            ms->possible_cpus->cpus[i].props.module_id = topo_ids.module_id;
        }
        ms->possible_cpus->cpus[i].props.has_core_id = true;
        ms->possible_cpus->cpus[i].props.core_id = topo_ids.core_id;
        ms->possible_cpus->cpus[i].props.has_thread_id = true;
        ms->possible_cpus->cpus[i].props.thread_id = topo_ids.smt_id;
    }

    return ms->possible_cpus;
}

static void x86_machine_stub_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);

    mc->possible_cpu_arch_ids = x86_machine_stub_possible_cpu_arch_ids;
}

static const TypeInfo x86_machine_stub_info = {
    .name = TYPE_X86_MACHINE,
    .parent = TYPE_MACHINE,
    .abstract = true,
    .instance_size = sizeof(X86MachineState),
    .class_size = sizeof(X86MachineClass),
    .class_init = x86_machine_stub_class_init,
    .interfaces = (InterfaceInfo[]) {
        { TYPE_HOTPLUG_HANDLER },
        { }
    },
};

static void x86_machine_stub_register_types(void)
{
    type_register_static(&x86_machine_stub_info);
}

type_init(x86_machine_stub_register_types)
