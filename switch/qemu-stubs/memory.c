/*
 * QEMU Stubs for Nintendo Switch - Memory Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Memory stubs - only used when NOT building the QEMU core.
 * When SWITCH_QEMU_CORE is defined, system/memory.c and system/physmem.c
 * provide the real implementations.
 */
#ifndef SWITCH_QEMU_CORE

/* Forward declarations for memory types */
typedef uint64_t hwaddr;
typedef uint64_t vaddr;
typedef struct AddressSpace AddressSpace;
typedef struct MemoryRegion MemoryRegion;
typedef struct MemoryRegionSection MemoryRegionSection;
typedef struct MemAccessCallback MemAccessCallback;
typedef struct CPUState CPUState;
typedef uint32_t MemTxResult;
typedef uint32_t MemOp;
typedef uint32_t MemTxAttrs;
typedef void (*MemAccessCallbackFunc)(void *opaque, MemoryRegion *mr, hwaddr addr, hwaddr len, bool write);

/* Constants */
#define MEMTX_OK 0

/* RAM list */
typedef struct RAMBlock RAMBlock;
typedef struct RAMList {
    QLIST_HEAD(, RAMBlock) blocks;
} RAMList;

/* Memory access functions */
uint8_t address_space_ldub(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint16_t address_space_lduw(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint16_t address_space_lduw_le(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint16_t address_space_lduw_be(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint32_t address_space_ldl(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint32_t address_space_ldl_le(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint32_t address_space_ldl_be(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint64_t address_space_ldq(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint64_t address_space_ldq_le(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

uint64_t address_space_ldq_be(AddressSpace *as, hwaddr addr, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)attrs; (void)result;
    return 0;
}

void address_space_stb(AddressSpace *as, hwaddr addr, uint8_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stw(AddressSpace *as, hwaddr addr, uint16_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stw_le(AddressSpace *as, hwaddr addr, uint16_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stw_be(AddressSpace *as, hwaddr addr, uint16_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stl(AddressSpace *as, hwaddr addr, uint32_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stl_le(AddressSpace *as, hwaddr addr, uint32_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stl_be(AddressSpace *as, hwaddr addr, uint32_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stq(AddressSpace *as, hwaddr addr, uint64_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stq_le(AddressSpace *as, hwaddr addr, uint64_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

void address_space_stq_be(AddressSpace *as, hwaddr addr, uint64_t val, MemTxAttrs attrs, MemTxResult *result)
{
    (void)as; (void)addr; (void)val; (void)attrs; (void)result;
}

AddressSpace *address_space_memory = NULL;

AddressSpace *address_space_io = NULL;

/* RAM list */
RAMList ram_list = { .blocks = QLIST_HEAD_INITIALIZER(ram_list.blocks) };

AddressSpace *cpu_get_address_space(CPUState *cpu, int asidx)
{
    (void)cpu; (void)asidx;
    return NULL;
}

/* Memory region functions */
uint64_t memory_region_size(MemoryRegion *mr)
{
    (void)mr;
    return 0;
}

void memory_region_set_client_dirty(MemoryRegion *mr, hwaddr addr, hwaddr size, unsigned client)
{
    (void)mr; (void)addr; (void)size; (void)client;
}

bool memory_region_test_and_clear_dirty(MemoryRegion *mr, hwaddr addr, hwaddr length, unsigned client)
{
    (void)mr; (void)addr; (void)length; (void)client;
    return false;
}

MemAccessCallback *mem_access_callback_insert(CPUState *cpu, MemoryRegion *mr, hwaddr offset, hwaddr len, MemAccessCallbackFunc func, void *opaque)
{
    (void)cpu; (void)mr; (void)offset; (void)len; (void)func; (void)opaque;
    return NULL;
}

void mem_access_callback_remove_by_ref(CPUState *cpu, MemAccessCallback *cb)
{
    (void)cpu; (void)cb;
}

MemTxResult memory_region_dispatch_read(MemoryRegion *mr, hwaddr addr, uint64_t *pval, MemOp op, MemTxAttrs attrs)
{
    (void)mr; (void)addr; (void)pval; (void)op; (void)attrs;
    return MEMTX_OK;
}

MemTxResult memory_region_dispatch_write(MemoryRegion *mr, hwaddr addr, uint64_t data, MemOp op, MemTxAttrs attrs)
{
    (void)mr; (void)addr; (void)data; (void)op; (void)attrs;
    return MEMTX_OK;
}

void *memory_region_get_ram_ptr(MemoryRegion *mr)
{
    (void)mr;
    return NULL;
}

hwaddr memory_region_get_ram_addr(MemoryRegion *mr)
{
    (void)mr;
    return 0;
}

MemoryRegionSection *iotlb_to_section(CPUArchState *env, hwaddr index, MemTxAttrs attrs)
{
    (void)env; (void)index; (void)attrs;
    return NULL;
}

hwaddr memory_region_section_get_iotlb(CPUState *cpu, MemoryRegionSection *section, hwaddr vaddr)
{
    (void)cpu; (void)section; (void)vaddr;
    return 0;
}

void mem_check_access_callback_vaddr(CPUState *cpu, vaddr addr, vaddr len, int flags, void *iotlbentry)
{
    (void)cpu; (void)addr; (void)len; (void)flags; (void)iotlbentry;
}

int mem_access_callback_address_matches(CPUState *cpu, hwaddr addr, hwaddr len)
{
    (void)cpu; (void)addr; (void)len;
    return 0;
}

hwaddr qemu_ram_addr_from_host_nofail(void *ptr)
{
    (void)ptr;
    return 0;
}

AddressSpace *address_space_translate_for_iotlb(CPUState *cpu, int asidx, hwaddr addr)
{
    (void)cpu; (void)asidx; (void)addr;
    return NULL;
}

/* Additional memory functions */
DeviceState *memory_region_owner(MemoryRegion *mr)
{
    (void)mr;
    return NULL;
}

void memory_region_init_io(MemoryRegion *mr, Object *owner, const MemoryRegionOps *ops, void *opaque, const char *name, uint64_t size)
{
    (void)mr; (void)owner; (void)ops; (void)opaque; (void)name; (void)size;
}

void memory_region_add_subregion(MemoryRegion *mr, hwaddr offset, MemoryRegion *subregion)
{
    (void)mr; (void)offset; (void)subregion;
}

void memory_region_del_subregion(MemoryRegion *mr, MemoryRegion *subregion)
{
    (void)mr; (void)subregion;
}

void memory_region_set_flush_coalesced(MemoryRegion *mr)
{
    (void)mr;
}

void memory_region_add_coalescing(MemoryRegion *mr, hwaddr offset, uint64_t size)
{
    (void)mr; (void)offset; (void)size;
}

void memory_region_init(MemoryRegion *mr, Object *owner, const char *name, uint64_t size)
{
    (void)mr; (void)owner; (void)name; (void)size;
}

bool memory_region_is_mapped(MemoryRegion *mr)
{
    (void)mr;
    return false;
}

void address_space_destroy(AddressSpace *as)
{
    (void)as;
}

void memory_region_add_subregion_overlap(MemoryRegion *mr, hwaddr offset, MemoryRegion *subregion, unsigned priority)
{
    (void)mr; (void)offset; (void)subregion; (void)priority;
}

void memory_region_set_enabled(MemoryRegion *mr, bool enabled)
{
    (void)mr; (void)enabled;
}

void memory_region_init_alias(MemoryRegion *mr, Object *owner, const char *name, MemoryRegion *orig, hwaddr offset, uint64_t size)
{
    (void)mr; (void)owner; (void)name; (void)orig; (void)offset; (void)size;
}

void address_space_init(AddressSpace *as, MemoryRegion *root, const char *name)
{
    (void)as; (void)root; (void)name;
}

void memory_region_init_rom(MemoryRegion *mr, Object *owner, const char *name, uint64_t size, Error **errp)
{
    (void)mr; (void)owner; (void)name; (void)size; (void)errp;
}

void memory_region_transaction_begin(void)
{
}

void memory_region_transaction_commit(void)
{
}

void memory_region_init_ram(MemoryRegion *mr, Object *owner, const char *name, uint64_t size, Error **errp)
{
    (void)mr; (void)owner; (void)name; (void)size; (void)errp;
}

void memory_region_set_readonly(MemoryRegion *mr, bool readonly)
{
    (void)mr; (void)readonly;
}

void memory_region_destroy(MemoryRegion *mr)
{
    (void)mr;
}

MemoryRegion *get_system_memory(void)
{
    return NULL;
}

MemoryRegion *get_system_io(void)
{
    return NULL;
}

void memory_region_set_log(MemoryRegion *mr, bool log, unsigned int log_size)
{
    (void)mr; (void)log; (void)log_size;
}

void memory_region_set_dirty(MemoryRegion *mr, hwaddr addr, hwaddr size)
{
    (void)mr; (void)addr; (void)size;
}

#endif /* !SWITCH_QEMU_CORE */