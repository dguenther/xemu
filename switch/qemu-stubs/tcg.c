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

void page_table_config_init(void)
{
    /* No-op on Switch */
}

unsigned int cpuinfo = 0;

void disas(FILE *out, const void *code, size_t size)
{
    (void)out;
    (void)code;
    (void)size;
}

void tb_invalidate_phys_range(uint64_t start, uint64_t end)
{
    (void)start;
    (void)end;
}

bool qtest_allowed = false;

void ram_block_notify_add(void *host, size_t size, size_t max_size)
{
    (void)host;
    (void)size;
    (void)max_size;
}

void ram_block_notify_remove(void *host, size_t size, uint32_t max_size)
{
    (void)host;
    (void)size;
    (void)max_size;
}

int cpu_get_phys_page_attrs_debug(void *cpu, uint64_t addr, void *attrs)
{
    (void)cpu;
    (void)addr;
    (void)attrs;
    return -1;
}

void finalize_target_page_bits(void)
{
}

bool set_preferred_target_page_bits(int bits)
{
    (void)bits;
    return true;
}

const char *lookup_symbol(uint64_t addr)
{
    (void)addr;
    return NULL;
}

void target_disas(FILE *out, void *cpu, uint64_t code, size_t size)
{
    (void)out;
    (void)cpu;
    (void)code;
    (void)size;
}

void tb_invalidate_phys_range_fast(uint64_t start, size_t len, uintptr_t ra)
{
    (void)start;
    (void)len;
    (void)ra;
}

void tb_unlock_pages(void *tb)
{
    (void)tb;
}

void tb_lock_page1(void *p1, void *p2)
{
    (void)p1;
    (void)p2;
}

void tb_unlock_page1(void *p1, void *p2)
{
    (void)p1;
    (void)p2;
}

void tb_flush(void *cpu)
{
    (void)cpu;
}

void tb_reset_jump(void *tb, int n)
{
    (void)tb;
    (void)n;
}

void *tb_link_page(void *tb, void *phys_pc, void *phys_page2)
{
    (void)phys_pc;
    (void)phys_page2;
    return tb;
}

void tb_lock_page0(void *p0)
{
    (void)p0;
}

int cpu_watchpoint_address_matches(void *cpu, uint64_t addr, uint64_t len)
{
    (void)cpu;
    (void)addr;
    (void)len;
    return 0;
}

void cpu_check_watchpoint(void *cpu, uint64_t addr, uint64_t len, int flags, int ra)
{
    (void)cpu;
    (void)addr;
    (void)len;
    (void)flags;
    (void)ra;
}

bool qtest_driver(void)
{
    return false;
}

void qtest_server_init(const char *qtest_chrdev, const char *qtest_log,
                       Error **errp)
{
    (void)qtest_chrdev;
    (void)qtest_log;
    (void)errp;
}

/* Additional TCG functions */
void tcg_iommu_init_notifier_list(CPUState *cpu)
{
    (void)cpu;
}

void tcg_iommu_free_notifier_list(CPUState *cpu)
{
    (void)cpu;
}

/* TCG allowed flag */
bool __attribute__((weak)) tcg_allowed(void)
{
    return false;
}
