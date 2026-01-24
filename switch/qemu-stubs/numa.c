/*
 * QEMU Stubs for Nintendo Switch - NUMA
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void parse_numa_opts(MachineState *ms)
{
    (void)ms;
}

void numa_complete_configuration(MachineState *ms)
{
    (void)ms;
}

bool numa_uses_legacy_mem(MachineState *ms)
{
    (void)ms;
    return false;
}
