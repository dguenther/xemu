/*
 * QEMU Stubs for Nintendo Switch - Threading and CPU Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"
#include "qemu/thread.h"
#include "sysemu/cpus.h"

/* Forward declarations */
typedef void (*VMChangeStateHandler)(void *opaque, int running, RunState state);
typedef int WakeupReason;

/* CPU threading functions */
void cpu_thread_signal_created(CPUState *cpu)
{
    (void)cpu;
}

void cpu_thread_signal_destroyed(CPUState *cpu)
{
    (void)cpu;
}

bool cpu_can_run(CPUState *cpu)
{
    (void)cpu;
    return true;
}

void cpu_handle_guest_debug(CPUState *cpu)
{
    (void)cpu;
}

bool cpu_work_list_empty(CPUState *cpu)
{
    (void)cpu;
    return true;
}

void qemu_cpu_kick(CPUState *cpu)
{
    (void)cpu;
}

bool qemu_in_vcpu_thread(void)
{
    return false;
}

void cpu_interrupt(CPUState *cpu, int mask)
{
    (void)cpu; (void)mask;
}

/* Watchpoint functions */
int cpu_watchpoint_insert(CPUState *cpu, vaddr addr, vaddr len, int flags, CPUWatchpoint **watchpoint)
{
    (void)cpu; (void)addr; (void)len; (void)flags; (void)watchpoint;
    return 0;
}

void cpu_watchpoint_remove_by_ref(CPUState *cpu, CPUWatchpoint *watchpoint)
{
    (void)cpu; (void)watchpoint;
}

/* Memory debug functions */
int cpu_memory_rw_debug(CPUState *cpu, vaddr addr, void *buf, size_t len, bool is_write)
{
    (void)cpu; (void)addr; (void)buf; (void)len; (void)is_write;
    return 0;
}

/* System functions */
void qemu_system_reset_request(ShutdownCause reason)
{
    (void)reason;
}

void qemu_system_shutdown_request(ShutdownCause reason)
{
    (void)reason;
}

/* Main thread functions */
bool qemu_in_main_thread(void)
{
    return true;
}

bool runstate_check(RunState state)
{
    (void)state;
    return true;
}

void qemu_add_vm_change_state_handler(VMChangeStateHandler *cb, void *opaque)
{
    (void)cb; (void)opaque;
}

void qemu_del_vm_change_state_handler(VMChangeStateHandler *cb, void *opaque)
{
    (void)cb; (void)opaque;
}

void qemu_system_vmstop_request_prepare(void)
{
}

void qemu_system_vmstop_request(RunState state)
{
    (void)state;
}

/* CPU functions */
void cpu_synchronize_post_init(CPUState *cpu)
{
    (void)cpu;
}

void cpu_resume(CPUState *cpu)
{
    (void)cpu;
}

void cpu_synchronize_state(CPUState *cpu)
{
    (void)cpu;
}

/* System control functions */
void qemu_system_wakeup_request(WakeupReason reason)
{
    (void)reason;
}

void qemu_system_suspend_request(void)
{
}

void qemu_system_wakeup_enable(WakeupReason reason)
{
    (void)reason;
}

/* CPU execution functions */
void cpu_exec_reset_hold(CPUState *cpu)
{
    (void)cpu;
}

/* Wakeup notifier functions */
void qemu_register_wakeup_notifier(Notifier *notifier)
{
    (void)notifier;
}

void qemu_register_wakeup_support(void)
{
}

/* CPU synchronization functions */
void cpu_synchronize_all_states(void)
{
}

void cpu_synchronize_all_post_reset(void)
{
}

/* KVM functions */
void kvm_reset_irq_delivered(void)
{
}

bool kvm_get_irq_delivered(void)
{
    return false;
}

/* Suspend notifier functions */
void qemu_register_suspend_notifier(Notifier *notifier)
{
    (void)notifier;
}

void cpu_watchpoint_remove_all(CPUState *cpu, int mask)
{
    (void)cpu; (void)mask;
}

bool qemu_cpu_is_self(CPUState *cpu)
{
    (void)cpu;
    return false;
}

int cpu_watchpoint_remove(CPUState *cpu, vaddr addr, vaddr len, int flags)
{
    (void)cpu; (void)addr; (void)len; (void)flags;
    return 0;
}

void cpu_remove_sync(CPUState *cpu)
{
    (void)cpu;
}

void qemu_init_vcpu(CPUState *cpu)
{
    (void)cpu;
}

/* x86 CPU functions */
void __attribute__((weak)) x86_cpu_after_reset(CPUState *cpu)
{
    (void)cpu;
}

/* TLB functions */
void __attribute__((weak)) tlb_flush(CPUState *cpu)
{
    (void)cpu;
}