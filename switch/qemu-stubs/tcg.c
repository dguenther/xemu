/*
 * QEMU Stubs for Nintendo Switch - TCG and CPU Execution
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * TCG and execution stubs
 */

unsigned int cpuinfo = 0;

void disas(FILE *out, const void *code, size_t size)
{
    switch_log("STUB: disas called\n");
    (void)out;
    (void)code;
    (void)size;
}

bool qtest_allowed = false;

__attribute__((weak)) int cpu_get_phys_page_attrs_debug(void *cpu,
                                                         uint64_t addr,
                                                         void *attrs)
{
    switch_log("STUB: cpu_get_phys_page_attrs_debug called\n");
    (void)cpu;
    (void)addr;
    (void)attrs;
    return -1;
}

void finalize_target_page_bits(void)
{
    switch_log("STUB: finalize_target_page_bits called\n");
}

bool set_preferred_target_page_bits(int bits)
{
    switch_log("STUB: set_preferred_target_page_bits called\n");
    (void)bits;
    return true;
}

const char *lookup_symbol(uint64_t addr)
{
    switch_log("STUB: lookup_symbol called\n");
    (void)addr;
    return NULL;
}

void target_disas(FILE *out, void *cpu, uint64_t code, size_t size)
{
    switch_log("STUB: target_disas called\n");
    (void)out;
    (void)cpu;
    (void)code;
    (void)size;
}

int cpu_watchpoint_address_matches(void *cpu, uint64_t addr, uint64_t len)
{
    switch_log("STUB: cpu_watchpoint_address_matches called\n");
    (void)cpu;
    (void)addr;
    (void)len;
    return 0;
}

void cpu_check_watchpoint(void *cpu, uint64_t addr, uint64_t len, int flags, int ra)
{
    switch_log("STUB: cpu_check_watchpoint called\n");
    (void)cpu;
    (void)addr;
    (void)len;
    (void)flags;
    (void)ra;
}

bool qtest_driver(void)
{
    switch_log("STUB: qtest_driver called\n");
    return false;
}

void qtest_server_init(const char *qtest_chrdev, const char *qtest_log,
                       Error **errp)
{
    switch_log("STUB: qtest_server_init called\n");
    (void)qtest_chrdev;
    (void)qtest_log;
    (void)errp;
}
