/*
 * QEMU Stubs for Nintendo Switch - GDB Server
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void gdb_init_cpu(CPUState *cpu)
{
    (void)cpu;
}

int gdbserver_start(const char *port_or_device)
{
    (void)port_or_device;
    return 0;
}

void gdb_exit(int code)
{
    (void)code;
}

void gdb_set_stop_cpu(void *cpu)
{
    (void)cpu;
}
