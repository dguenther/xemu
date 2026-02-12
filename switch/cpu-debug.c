/*
 * Switch CPU progress diagnostics.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/atomic.h"
#include "hw/core/cpu.h"
#include "exec/cpu-common.h"
#if defined(TARGET_I386)
#include "target/i386/cpu.h"
#endif

#ifndef SWITCH_CPU_DETAIL_LOGS
#define SWITCH_CPU_DETAIL_LOGS 0
#endif

extern void switch_log(const char *format, ...);

typedef struct SwitchNV2ABlockProbeCounters {
    uint64_t pfifo_total;
    uint64_t pgraph_total;
    uint64_t pcrtc_total;
    uint64_t pfifo_post_kernel;
    uint64_t pgraph_post_kernel;
    uint64_t pcrtc_post_kernel;
    uint64_t pfifo_last_post_kernel_pc;
    uint64_t pgraph_last_post_kernel_pc;
    uint64_t pcrtc_last_post_kernel_pc;
} SwitchNV2ABlockProbeCounters;

static SwitchNV2ABlockProbeCounters g_nv2a_block_probe;

enum {
    SWITCH_IO_GROUP_PCI_CFG = 0,
    SWITCH_IO_GROUP_IDE,
    SWITCH_IO_GROUP_SMBUS,
    SWITCH_IO_GROUP_PIC,
    SWITCH_IO_GROUP_PIT,
    SWITCH_IO_GROUP_RTC,
    SWITCH_IO_GROUP_POST,
    SWITCH_IO_GROUP_VGA,
    SWITCH_IO_GROUP_OTHER,
    SWITCH_IO_GROUP_COUNT
};

enum {
    SWITCH_MMIO_GROUP_NV2A = 0,
    SWITCH_MMIO_GROUP_APU,
    SWITCH_MMIO_GROUP_ACI,
    SWITCH_MMIO_GROUP_SMBUS,
    SWITCH_MMIO_GROUP_PCI,
    SWITCH_MMIO_GROUP_LPC,
    SWITCH_MMIO_GROUP_APIC,
    SWITCH_MMIO_GROUP_OTHER,
    SWITCH_MMIO_GROUP_COUNT
};

typedef struct SwitchStallIOProbeCounters {
    uint64_t total_reads;
    uint64_t total_writes;
    uint64_t groups[SWITCH_IO_GROUP_COUNT];
    uint64_t last_eip;
    uint64_t last_port;
    uint64_t last_size;
    uint64_t last_value;
    uint64_t last_is_write;
    uint64_t last_seq;
} SwitchStallIOProbeCounters;

static SwitchStallIOProbeCounters g_stall_io_probe;

typedef struct SwitchStallMMIOProbeCounters {
    uint64_t total_reads;
    uint64_t total_writes;
    uint64_t groups[SWITCH_MMIO_GROUP_COUNT];
    uint64_t last_eip;
    uint64_t last_vaddr;
    uint64_t last_offset;
    uint64_t last_size;
    uint64_t last_value;
    uint64_t last_is_write;
    uint64_t last_group;
    uint64_t last_seq;
} SwitchStallMMIOProbeCounters;

static SwitchStallMMIOProbeCounters g_stall_mmio_probe;

typedef struct SwitchStallMemProbeCounters {
    uint64_t total_reads;
    uint64_t total_writes;
    uint64_t last_eip;
    uint64_t last_vaddr;
    uint64_t last_size;
    uint64_t last_value;
    uint64_t last_is_write;
    uint64_t last_seq;
} SwitchStallMemProbeCounters;

static SwitchStallMemProbeCounters g_stall_mem_probe;

static bool switch_in_high_stall_window(vaddr pc)
{
    return pc >= 0x80054000ull && pc < 0x80056000ull;
}

static int switch_mmio_group_for_region(const char *mr_name)
{
    if (!mr_name) {
        return SWITCH_MMIO_GROUP_OTHER;
    }
    if (strstr(mr_name, "nv2a") || strstr(mr_name, "PFB") ||
        strstr(mr_name, "PBUS") || strstr(mr_name, "PFIFO") ||
        strstr(mr_name, "PGRAPH") || strstr(mr_name, "PCRTC")) {
        return SWITCH_MMIO_GROUP_NV2A;
    }
    if (strstr(mr_name, "apu")) {
        return SWITCH_MMIO_GROUP_APU;
    }
    if (strstr(mr_name, "aci")) {
        return SWITCH_MMIO_GROUP_ACI;
    }
    if (strstr(mr_name, "smb") || strstr(mr_name, "SMB")) {
        return SWITCH_MMIO_GROUP_SMBUS;
    }
    if (strstr(mr_name, "pci")) {
        return SWITCH_MMIO_GROUP_PCI;
    }
    if (strstr(mr_name, "lpc")) {
        return SWITCH_MMIO_GROUP_LPC;
    }
    if (strstr(mr_name, "apic")) {
        return SWITCH_MMIO_GROUP_APIC;
    }
    return SWITCH_MMIO_GROUP_OTHER;
}

static int switch_io_group_for_port(uint32_t port)
{
    if (port >= 0xcf8 && port <= 0xcff) {
        return SWITCH_IO_GROUP_PCI_CFG;
    }
    if ((port >= 0x1f0 && port <= 0x1f7) || port == 0x3f6 ||
        (port >= 0x170 && port <= 0x177) || port == 0x376) {
        return SWITCH_IO_GROUP_IDE;
    }
    if (port >= 0xc000 && port <= 0xc2ff) {
        return SWITCH_IO_GROUP_SMBUS;
    }
    if ((port >= 0x20 && port <= 0x21) || (port >= 0xa0 && port <= 0xa1)) {
        return SWITCH_IO_GROUP_PIC;
    }
    if (port >= 0x40 && port <= 0x43) {
        return SWITCH_IO_GROUP_PIT;
    }
    if (port >= 0x70 && port <= 0x71) {
        return SWITCH_IO_GROUP_RTC;
    }
    if (port == 0x80) {
        return SWITCH_IO_GROUP_POST;
    }
    if (port >= 0x3b0 && port <= 0x3df) {
        return SWITCH_IO_GROUP_VGA;
    }
    return SWITCH_IO_GROUP_OTHER;
}

void switch_debug_note_io_port_access(uint32_t eip, uint32_t port,
                                      bool is_write, unsigned size,
                                      uint32_t value)
{
    if (!switch_in_high_stall_window(eip)) {
        return;
    }

    if (is_write) {
        qatomic_inc(&g_stall_io_probe.total_writes);
    } else {
        qatomic_inc(&g_stall_io_probe.total_reads);
    }

    int group = switch_io_group_for_port(port);
    if (group >= 0 && group < SWITCH_IO_GROUP_COUNT) {
        qatomic_inc(&g_stall_io_probe.groups[group]);
    }

    qatomic_set(&g_stall_io_probe.last_eip, eip);
    qatomic_set(&g_stall_io_probe.last_port, port & 0xffffu);
    qatomic_set(&g_stall_io_probe.last_size, size);
    qatomic_set(&g_stall_io_probe.last_value, value);
    qatomic_set(&g_stall_io_probe.last_is_write, is_write ? 1 : 0);
    qatomic_inc(&g_stall_io_probe.last_seq);
}

void switch_debug_note_mmio_access(vaddr eip, vaddr addr, hwaddr mr_offset,
                                   bool is_write, unsigned size,
                                   uint64_t value, const char *mr_name)
{
    if (!switch_in_high_stall_window(eip)) {
        return;
    }

    if (is_write) {
        qatomic_inc(&g_stall_mmio_probe.total_writes);
    } else {
        qatomic_inc(&g_stall_mmio_probe.total_reads);
    }

    int group = switch_mmio_group_for_region(mr_name);
    if (group >= 0 && group < SWITCH_MMIO_GROUP_COUNT) {
        qatomic_inc(&g_stall_mmio_probe.groups[group]);
    }

    qatomic_set(&g_stall_mmio_probe.last_eip, (uint64_t)eip);
    qatomic_set(&g_stall_mmio_probe.last_vaddr, (uint64_t)addr);
    qatomic_set(&g_stall_mmio_probe.last_offset, (uint64_t)mr_offset);
    qatomic_set(&g_stall_mmio_probe.last_size, size);
    qatomic_set(&g_stall_mmio_probe.last_value, value);
    qatomic_set(&g_stall_mmio_probe.last_is_write, is_write ? 1 : 0);
    qatomic_set(&g_stall_mmio_probe.last_group, group);
    qatomic_inc(&g_stall_mmio_probe.last_seq);
}

void switch_debug_note_mem_access(vaddr eip, vaddr addr, bool is_write,
                                  unsigned size, uint64_t value)
{
    if (!switch_in_high_stall_window(eip)) {
        return;
    }

    if (is_write) {
        qatomic_inc(&g_stall_mem_probe.total_writes);
    } else {
        qatomic_inc(&g_stall_mem_probe.total_reads);
    }

    qatomic_set(&g_stall_mem_probe.last_eip, (uint64_t)eip);
    qatomic_set(&g_stall_mem_probe.last_vaddr, (uint64_t)addr);
    qatomic_set(&g_stall_mem_probe.last_size, size);
    qatomic_set(&g_stall_mem_probe.last_value, value);
    qatomic_set(&g_stall_mem_probe.last_is_write, is_write ? 1 : 0);
    qatomic_inc(&g_stall_mem_probe.last_seq);
}

void switch_debug_note_nv2a_block_write(bool pfifo, bool pgraph, bool pcrtc,
                                        bool post_kernel, vaddr pc)
{
    uint64_t pc_u64 = (uint64_t)pc;

    if (pfifo) {
        qatomic_inc(&g_nv2a_block_probe.pfifo_total);
        if (post_kernel) {
            qatomic_inc(&g_nv2a_block_probe.pfifo_post_kernel);
            qatomic_set(&g_nv2a_block_probe.pfifo_last_post_kernel_pc, pc_u64);
        }
    }
    if (pgraph) {
        qatomic_inc(&g_nv2a_block_probe.pgraph_total);
        if (post_kernel) {
            qatomic_inc(&g_nv2a_block_probe.pgraph_post_kernel);
            qatomic_set(&g_nv2a_block_probe.pgraph_last_post_kernel_pc, pc_u64);
        }
    }
    if (pcrtc) {
        qatomic_inc(&g_nv2a_block_probe.pcrtc_total);
        if (post_kernel) {
            qatomic_inc(&g_nv2a_block_probe.pcrtc_post_kernel);
            qatomic_set(&g_nv2a_block_probe.pcrtc_last_post_kernel_pc, pc_u64);
        }
    }
}

static const char *switch_pc_region_name(vaddr pc)
{
    if (pc >= 0xfffffe00ull) {
        return "mcpx-rom";
    }
    if (pc >= 0x00090000ull && pc < 0x000a0000ull) {
        return "mcpx-stage2";
    }
    if (pc >= 0xff000000ull) {
        return "bios-rom";
    }
    if (pc < 0x00100000ull) {
        return "lowmem";
    }
    return "other";
}

void switch_debug_log_cpu0_state(void)
{
    static bool have_prev_pc;
    static vaddr prev_pc;
    static const char *prev_region;
    static unsigned sample_count;
    static unsigned stagnant_samples;

#if defined(TARGET_I386)
    static bool hit_401k;
    static bool hit_402k;
    static bool hit_403k;
#if SWITCH_CPU_DETAIL_LOGS
    static vaddr last_detail_pc;
    static unsigned last_detail_stagnant;
    static unsigned last_detail_sample;
#endif
#endif

    CPUState *cpu = qemu_get_cpu(0);
    vaddr pc = 0;

    if (!cpu) {
        if (sample_count < 10 || (sample_count % 120) == 0) {
            switch_log("Switch: cpu0 state unavailable (no CPU 0)\n");
        }
        sample_count++;
        return;
    }

    if (cpu->cc && cpu->cc->get_pc) {
        pc = cpu->cc->get_pc(cpu);
    }

    const char *region = switch_pc_region_name(pc);
    bool region_changed = !prev_region || strcmp(prev_region, region) != 0;

    if (have_prev_pc && pc == prev_pc) {
        stagnant_samples++;
    } else {
        stagnant_samples = 0;
    }
    prev_pc = pc;
    have_prev_pc = true;
    prev_region = region;

    if (region_changed) {
        switch_log("Switch: cpu0 region -> %s pc=0x%" VADDR_PRIx "\n",
                   region, pc);
    }

    if (sample_count < 20 || (sample_count % 30) == 0 ||
        stagnant_samples == 60 || stagnant_samples == 300) {
        uint64_t pfifo_total = qatomic_read(&g_nv2a_block_probe.pfifo_total);
        uint64_t pgraph_total = qatomic_read(&g_nv2a_block_probe.pgraph_total);
        uint64_t pcrtc_total = qatomic_read(&g_nv2a_block_probe.pcrtc_total);
        uint64_t pfifo_post = qatomic_read(
            &g_nv2a_block_probe.pfifo_post_kernel);
        uint64_t pgraph_post = qatomic_read(
            &g_nv2a_block_probe.pgraph_post_kernel);
        uint64_t pcrtc_post = qatomic_read(
            &g_nv2a_block_probe.pcrtc_post_kernel);
        uint64_t pfifo_last_pc = qatomic_read(
            &g_nv2a_block_probe.pfifo_last_post_kernel_pc);
        uint64_t pgraph_last_pc = qatomic_read(
            &g_nv2a_block_probe.pgraph_last_post_kernel_pc);
        uint64_t pcrtc_last_pc = qatomic_read(
            &g_nv2a_block_probe.pcrtc_last_post_kernel_pc);

        switch_log(
            "Switch: cpu0 pc=0x%" VADDR_PRIx " region=%s halted=%d stopped=%d int_req=0x%x stagnant=%u\n",
            pc,
            region,
            cpu->halted ? 1 : 0,
            cpu->stopped ? 1 : 0,
            cpu->interrupt_request,
            stagnant_samples);
        switch_log(
            "Switch: nv2a writes total pfifo=%" PRIu64 " pgraph=%" PRIu64 " pcrtc=%" PRIu64
            " post-kernel pfifo=%" PRIu64 "@0x%08" PRIx64
            " pgraph=%" PRIu64 "@0x%08" PRIx64
            " pcrtc=%" PRIu64 "@0x%08" PRIx64 "\n",
            pfifo_total, pgraph_total, pcrtc_total,
            pfifo_post, pfifo_last_pc,
            pgraph_post, pgraph_last_pc,
            pcrtc_post, pcrtc_last_pc);
    }

#if defined(TARGET_I386)
    bool in_bios_boot_window = (pc >= 0x00400000ull && pc < 0x00800000ull);
    bool in_late_high_window = switch_in_high_stall_window(pc);
    if (!hit_401k && in_bios_boot_window && pc >= 0x00401000ull) {
        hit_401k = true;
        switch_log("Switch: cpu0 milestone reached pc>=0x401000 (pc=0x%" VADDR_PRIx ")\n",
                   pc);
    }
    if (!hit_402k && in_bios_boot_window && pc >= 0x00402000ull) {
        hit_402k = true;
        switch_log("Switch: cpu0 milestone reached pc>=0x402000 (pc=0x%" VADDR_PRIx ")\n",
                   pc);
    }
    if (!hit_403k && in_bios_boot_window && pc >= 0x00403000ull) {
        hit_403k = true;
        switch_log("Switch: cpu0 milestone reached pc>=0x403000 (pc=0x%" VADDR_PRIx ")\n",
                   pc);
    }

    if (pc == 0x00400efbull &&
        (stagnant_samples == 60 || stagnant_samples == 180 ||
         stagnant_samples == 300 || (stagnant_samples > 0 &&
         (stagnant_samples % 600) == 0))) {
        switch_log("Switch: cpu0 stall marker at 0x400efb stagnant=%u int_req=0x%x\n",
                   stagnant_samples, cpu->interrupt_request);
    }

    if (in_late_high_window &&
        (stagnant_samples == 16 || stagnant_samples == 60 ||
         stagnant_samples == 180 || stagnant_samples == 300 ||
         (stagnant_samples > 0 && (stagnant_samples % 600) == 0))) {
        static uint64_t last_io_seq_logged;
        static uint64_t last_mmio_seq_logged;
        static uint64_t last_mem_seq_logged;
        static vaddr last_stall_code_pc;
        static unsigned last_stall_code_stagnant;
        uint64_t io_reads = qatomic_read(&g_stall_io_probe.total_reads);
        uint64_t io_writes = qatomic_read(&g_stall_io_probe.total_writes);
        uint64_t io_last_eip = qatomic_read(&g_stall_io_probe.last_eip);
        uint64_t io_last_port = qatomic_read(&g_stall_io_probe.last_port);
        uint64_t io_last_size = qatomic_read(&g_stall_io_probe.last_size);
        uint64_t io_last_value = qatomic_read(&g_stall_io_probe.last_value);
        uint64_t io_last_is_write = qatomic_read(&g_stall_io_probe.last_is_write);
        uint64_t io_last_seq = qatomic_read(&g_stall_io_probe.last_seq);

        uint64_t io_pci = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_PCI_CFG]);
        uint64_t io_ide = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_IDE]);
        uint64_t io_smbus = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_SMBUS]);
        uint64_t io_pic = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_PIC]);
        uint64_t io_pit = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_PIT]);
        uint64_t io_rtc = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_RTC]);
        uint64_t io_post = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_POST]);
        uint64_t io_vga = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_VGA]);
        uint64_t io_other = qatomic_read(&g_stall_io_probe.groups[SWITCH_IO_GROUP_OTHER]);

        uint64_t mmio_reads = qatomic_read(&g_stall_mmio_probe.total_reads);
        uint64_t mmio_writes = qatomic_read(&g_stall_mmio_probe.total_writes);
        uint64_t mmio_last_eip = qatomic_read(&g_stall_mmio_probe.last_eip);
        uint64_t mmio_last_vaddr = qatomic_read(&g_stall_mmio_probe.last_vaddr);
        uint64_t mmio_last_offset = qatomic_read(&g_stall_mmio_probe.last_offset);
        uint64_t mmio_last_size = qatomic_read(&g_stall_mmio_probe.last_size);
        uint64_t mmio_last_value = qatomic_read(&g_stall_mmio_probe.last_value);
        uint64_t mmio_last_is_write = qatomic_read(&g_stall_mmio_probe.last_is_write);
        uint64_t mmio_last_group = qatomic_read(&g_stall_mmio_probe.last_group);
        uint64_t mmio_last_seq = qatomic_read(&g_stall_mmio_probe.last_seq);

        uint64_t mmio_nv2a = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_NV2A]);
        uint64_t mmio_apu = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_APU]);
        uint64_t mmio_aci = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_ACI]);
        uint64_t mmio_smbus = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_SMBUS]);
        uint64_t mmio_pci = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_PCI]);
        uint64_t mmio_lpc = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_LPC]);
        uint64_t mmio_apic = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_APIC]);
        uint64_t mmio_other = qatomic_read(&g_stall_mmio_probe.groups[SWITCH_MMIO_GROUP_OTHER]);

        uint64_t mem_reads = qatomic_read(&g_stall_mem_probe.total_reads);
        uint64_t mem_writes = qatomic_read(&g_stall_mem_probe.total_writes);
        uint64_t mem_last_eip = qatomic_read(&g_stall_mem_probe.last_eip);
        uint64_t mem_last_vaddr = qatomic_read(&g_stall_mem_probe.last_vaddr);
        uint64_t mem_last_size = qatomic_read(&g_stall_mem_probe.last_size);
        uint64_t mem_last_value = qatomic_read(&g_stall_mem_probe.last_value);
        uint64_t mem_last_is_write = qatomic_read(&g_stall_mem_probe.last_is_write);
        uint64_t mem_last_seq = qatomic_read(&g_stall_mem_probe.last_seq);

        switch_log("Switch: cpu0 high stall window pc=0x%" VADDR_PRIx
                   " stagnant=%u int_req=0x%x\n",
                   pc, stagnant_samples, cpu->interrupt_request);
        switch_log(
            "Switch: stall-io reads=%" PRIu64 " writes=%" PRIu64
            " pci=%" PRIu64 " ide=%" PRIu64 " smbus=%" PRIu64
            " pic=%" PRIu64 " pit=%" PRIu64 " rtc=%" PRIu64
            " post=%" PRIu64 " vga=%" PRIu64 " other=%" PRIu64 "\n",
            io_reads, io_writes,
            io_pci, io_ide, io_smbus,
            io_pic, io_pit, io_rtc,
            io_post, io_vga, io_other);
        switch_log(
            "Switch: stall-mmio reads=%" PRIu64 " writes=%" PRIu64
            " nv2a=%" PRIu64 " apu=%" PRIu64 " aci=%" PRIu64
            " smbus=%" PRIu64 " pci=%" PRIu64 " lpc=%" PRIu64
            " apic=%" PRIu64 " other=%" PRIu64 "\n",
            mmio_reads, mmio_writes,
            mmio_nv2a, mmio_apu, mmio_aci,
            mmio_smbus, mmio_pci, mmio_lpc,
            mmio_apic, mmio_other);
        switch_log(
            "Switch: stall-mem reads=%" PRIu64 " writes=%" PRIu64 "\n",
            mem_reads, mem_writes);

        if (io_last_seq != last_io_seq_logged) {
            switch_log(
                "Switch: stall-io last %s%u port=0x%04" PRIx64
                " val=0x%08" PRIx64 " eip=0x%08" PRIx64 " seq=%" PRIu64 "\n",
                io_last_is_write ? "out" : "in",
                (unsigned)io_last_size,
                io_last_port,
                io_last_value,
                io_last_eip,
                io_last_seq);
            last_io_seq_logged = io_last_seq;
        }
        if (mmio_last_seq != last_mmio_seq_logged) {
            switch_log(
                "Switch: stall-mmio last %s%u grp=%" PRIu64
                " vaddr=0x%08" PRIx64 " off=0x%08" PRIx64
                " val=0x%08" PRIx64 " eip=0x%08" PRIx64 " seq=%" PRIu64 "\n",
                mmio_last_is_write ? "st" : "ld",
                (unsigned)mmio_last_size,
                mmio_last_group,
                mmio_last_vaddr,
                mmio_last_offset,
                mmio_last_value,
                mmio_last_eip,
                mmio_last_seq);
            last_mmio_seq_logged = mmio_last_seq;
        }
        if (mem_last_seq != last_mem_seq_logged) {
            switch_log(
                "Switch: stall-mem last %s%u vaddr=0x%08" PRIx64
                " page=0x%08" PRIx64 " val=0x%08" PRIx64
                " eip=0x%08" PRIx64 " seq=%" PRIu64 "\n",
                mem_last_is_write ? "st" : "ld",
                (unsigned)mem_last_size,
                mem_last_vaddr,
                mem_last_vaddr & ~0xfffull,
                mem_last_value,
                mem_last_eip,
                mem_last_seq);
            last_mem_seq_logged = mem_last_seq;
        }

        if (pc != last_stall_code_pc ||
            stagnant_samples >= last_stall_code_stagnant + 600 ||
            stagnant_samples == 16) {
            X86CPU *x86 = X86_CPU(cpu);
            CPUX86State *env = &x86->env;
            uint8_t insn[8] = {0};
            int rc = cpu_memory_rw_debug(cpu, pc, insn, sizeof(insn), 0);
            if (rc == 0) {
                switch_log(
                    "Switch: stall-code pc=0x%08" VADDR_PRIx
                    " eflags=0x%08x eax=0x%08x ecx=0x%08x edx=0x%08x"
                    " esi=0x%08x edi=0x%08x esp=0x%08x bytes=%02x %02x %02x %02x %02x %02x %02x %02x\n",
                    pc,
                    env->eflags,
                    (uint32_t)env->regs[R_EAX],
                    (uint32_t)env->regs[R_ECX],
                    (uint32_t)env->regs[R_EDX],
                    (uint32_t)env->regs[R_ESI],
                    (uint32_t)env->regs[R_EDI],
                    (uint32_t)env->regs[R_ESP],
                    insn[0], insn[1], insn[2], insn[3],
                    insn[4], insn[5], insn[6], insn[7]);
            } else {
                switch_log("Switch: stall-code read failed pc=0x%08" VADDR_PRIx
                           " rc=%d\n", pc, rc);
            }
            last_stall_code_pc = pc;
            last_stall_code_stagnant = stagnant_samples;
        }
    }

#if SWITCH_CPU_DETAIL_LOGS
    bool in_bios_4m_window = (pc >= 0x00400000ull && pc < 0x00410000ull);
    bool in_probe_loop_window = (pc >= 0x00400900ull && pc < 0x00400d00ull);
    bool in_late_boot_window = (pc >= 0x00401000ull && pc < 0x00404000ull);
    bool in_high_stall_window = (pc >= 0x80054000ull && pc < 0x80056000ull);
    bool should_log_detail =
        (stagnant_samples >= 16 && in_bios_4m_window) ||
        (stagnant_samples >= 4 && in_probe_loop_window) ||
        (stagnant_samples >= 4 && in_late_boot_window) ||
        (stagnant_samples >= 8 && in_high_stall_window);

    if (should_log_detail) {
        bool detailed = (pc != last_detail_pc) ||
                        (stagnant_samples >= last_detail_stagnant + 32) ||
                        (in_probe_loop_window &&
                         sample_count >= last_detail_sample + 12) ||
                        (in_late_boot_window &&
                         sample_count >= last_detail_sample + 12) ||
                        (in_high_stall_window &&
                         sample_count >= last_detail_sample + 8);
        if (detailed) {
            X86CPU *x86 = X86_CPU(cpu);
            CPUX86State *env = &x86->env;
            uint8_t insn[16] = {0};
            int rc = cpu_memory_rw_debug(cpu, pc, insn, sizeof(insn), 0);

            switch_log(
                "Switch: cpu0 regs eip=0x%08x eflags=0x%08x eax=0x%08x ebx=0x%08x ecx=0x%08x edx=0x%08x esi=0x%08x edi=0x%08x esp=0x%08x ebp=0x%08x\n",
                env->eip,
                env->eflags,
                (uint32_t)env->regs[R_EAX],
                (uint32_t)env->regs[R_EBX],
                (uint32_t)env->regs[R_ECX],
                (uint32_t)env->regs[R_EDX],
                (uint32_t)env->regs[R_ESI],
                (uint32_t)env->regs[R_EDI],
                (uint32_t)env->regs[R_ESP],
                (uint32_t)env->regs[R_EBP]);
            switch_log("Switch: cpu0 ctrl cr0=0x%08x cr3=0x%08x cr4=0x%08x\n",
                       env->cr[0], env->cr[3], env->cr[4]);

            if (rc == 0) {
                switch_log(
                    "Switch: cpu0 code pc=0x%08" VADDR_PRIx " bytes=%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
                    pc,
                    insn[0], insn[1], insn[2], insn[3],
                    insn[4], insn[5], insn[6], insn[7],
                    insn[8], insn[9], insn[10], insn[11],
                    insn[12], insn[13], insn[14], insn[15]);
            } else {
                switch_log("Switch: cpu0 code read failed pc=0x%08" VADDR_PRIx " rc=%d\n",
                           pc, rc);
            }

            if (pc >= 0x00401000ull && pc < 0x00404000ull) {
                uint32_t stack_words[4] = {0};
                int src_rc = cpu_memory_rw_debug(cpu,
                                                 (vaddr)env->regs[R_ESI],
                                                 (uint8_t *)&stack_words[0],
                                                 sizeof(uint32_t), 0);
                int dst_rc = cpu_memory_rw_debug(cpu,
                                                 (vaddr)env->regs[R_EDI],
                                                 (uint8_t *)&stack_words[1],
                                                 sizeof(uint32_t), 0);
                int esp_rc = cpu_memory_rw_debug(cpu,
                                                 (vaddr)env->regs[R_ESP],
                                                 (uint8_t *)&stack_words[2],
                                                 sizeof(uint32_t) * 2, 0);
                uint32_t ebx_word = 0;
                int ebx_rc = cpu_memory_rw_debug(cpu,
                                                 (vaddr)env->regs[R_EBX],
                                                 (uint8_t *)&ebx_word,
                                                 sizeof(uint32_t), 0);
                if (src_rc == 0 && dst_rc == 0 && esp_rc == 0) {
                    switch_log(
                        "Switch: cpu0 mem esi[0]=0x%08x edi[0]=0x%08x ebx[0]=0x%08x ret=0x%08x next=0x%08x\n",
                        stack_words[0], stack_words[1],
                        (ebx_rc == 0) ? ebx_word : 0xffffffffu,
                        stack_words[2], stack_words[3]);
                } else {
                    switch_log(
                        "Switch: cpu0 mem read rc src=%d dst=%d ebx=%d esp=%d\n",
                        src_rc, dst_rc, ebx_rc, esp_rc);
                }
            }

            last_detail_pc = pc;
            last_detail_stagnant = stagnant_samples;
            last_detail_sample = sample_count;
        }
    }
#endif
#endif

    sample_count++;
}
