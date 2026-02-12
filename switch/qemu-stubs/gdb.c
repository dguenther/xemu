/*
 * QEMU Stubs for Nintendo Switch - GDB Server
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void gdb_init_cpu(CPUState *cpu)
{
    switch_log("STUB: gdb_init_cpu called\n");
    (void)cpu;
}

int gdbserver_start(const char *port_or_device)
{
    switch_log("STUB: gdbserver_start called\n");
    (void)port_or_device;
    return 0;
}

void gdb_exit(int code)
{
    switch_log("STUB: gdb_exit called\n");
    (void)code;
}

void gdb_set_stop_cpu(void *cpu)
{
    switch_log("STUB: gdb_set_stop_cpu called\n");
    (void)cpu;
}
