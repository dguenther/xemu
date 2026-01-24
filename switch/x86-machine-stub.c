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

static const TypeInfo x86_machine_stub_info = {
    .name = TYPE_X86_MACHINE,
    .parent = TYPE_MACHINE,
    .abstract = true,
    .instance_size = sizeof(X86MachineState),
    .class_size = sizeof(X86MachineClass),
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
