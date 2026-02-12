/*
 * QEMU Stubs for Nintendo Switch - NUMA
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void parse_numa_opts(MachineState *ms)
{
    switch_log("STUB: parse_numa_opts called\n");
    (void)ms;
}

void numa_complete_configuration(MachineState *ms)
{
    switch_log("STUB: numa_complete_configuration called\n");
    (void)ms;
}

bool numa_uses_legacy_mem(MachineState *ms)
{
    switch_log("STUB: numa_uses_legacy_mem called\n");
    (void)ms;
    return false;
}
