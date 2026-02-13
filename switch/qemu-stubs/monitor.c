/*
 * QEMU Stubs for Nintendo Switch - Monitor
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"
#include "monitor/monitor.h"
#include "disas/disas.h"

/*
 * Monitor stubs
 */

void monitor_init_hmp(Chardev *chr, bool use_readline, Error **errp)
{
    switch_log("STUB: monitor_init_hmp called\n");
    (void)chr;
    (void)use_readline;
    (void)errp;
}

typedef struct AddfdInfo AddfdInfo;

void monitor_register_hmp_info_hrt(const char *name,
                                   HumanReadableText *(*handler)(Error **errp))
{
    (void)name;
    (void)handler;
}

void handle_hmp_command(MonitorHMP *mon, const char *cmdline)
{
    (void)mon;
    (void)cmdline;
}

bool hmp_handle_error(Monitor *mon, Error *err)
{
    (void)mon;
    if (err) {
        error_free(err);
    }
    return false;
}

void monitor_disas(Monitor *mon, CPUState *cpu, uint64_t pc,
                   int nb_insn, bool is_physical)
{
    (void)mon;
    (void)cpu;
    (void)pc;
    (void)nb_insn;
    (void)is_physical;
}

bool qemu_system_dump_in_progress(void)
{
    return false;
}
