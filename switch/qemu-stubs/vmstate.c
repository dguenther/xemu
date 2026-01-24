/*
 * QEMU Stubs for Nintendo Switch - VMState and Migration
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * VMState info stubs
 */
__attribute__((weak)) const VMStateInfo vmstate_info_uint16 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_uint8 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_int64 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_timer = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_bool = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_int16 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_int32 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_uint32 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_uint64 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_unused_buffer = { 0 };
extern const void *vmstate_info_buffer;
const void *vmstate_info_buffer = NULL;

static int vmstate_stub_get(QEMUFile *f, void *pv, size_t size,
                            const VMStateField *field)
{
    (void)f;
    (void)pv;
    (void)size;
    (void)field;
    return 0;
}

static int vmstate_stub_put(QEMUFile *f, void *pv, size_t size,
                            const VMStateField *field, JSONWriter *vmdesc)
{
    (void)f;
    (void)pv;
    (void)size;
    (void)field;
    (void)vmdesc;
    return 0;
}

const VMStateInfo vmstate_info_int32_le = {
    .name = "int32_le",
    .get = vmstate_stub_get,
    .put = vmstate_stub_put,
};

const VMStateInfo vmstate_info_int32_equal = {
    .name = "int32_equal",
    .get = vmstate_stub_get,
    .put = vmstate_stub_put,
};

__attribute__((weak)) const VMStateDescription vmstate_pci_device = { 0 };
__attribute__((weak)) const VMStateDescription vmstate_serial = { 0 };
__attribute__((weak)) const VMStateDescription vmstate_vga_common = { 0 };
__attribute__((weak)) const VMStateDescription vmstate_x86_cpu = { 0 };

/* Stub implementation - always allow devices (no migration on Switch) */
bool vmstate_check_only_migratable(const VMStateDescription *vmsd)
{
    (void)vmsd;
    return true;
}

void vmstate_register_ram_global(MemoryRegion *mr)
{
    (void)mr;
}

void vmstate_unregister_ram(MemoryRegion *mr, DeviceState *dev)
{
    (void)mr;
    (void)dev;
}

void qemu_put_buffer(QEMUFile *f, const uint8_t *buf, size_t size)
{
    (void)f;
    (void)buf;
    (void)size;
}

size_t qemu_get_buffer(QEMUFile *f, uint8_t *buf, size_t size)
{
    (void)f;
    if (buf && size) {
        memset(buf, 0, size);
    }
    return size;
}

void qemu_put_be32(QEMUFile *f, unsigned int v)
{
    (void)f;
    (void)v;
}

unsigned int qemu_get_be32(QEMUFile *f)
{
    (void)f;
    return 0;
}

bool load_snapshot(const char *name, const char *vmstate,
                   bool has_devices, strList *devices,
                   Error **errp)
{
    (void)name;
    (void)vmstate;
    (void)has_devices;
    (void)devices;
    (void)errp;
    return false;
}

void load_snapshot_resume(int state)
{
    (void)state;
}

void migration_object_init(void)
{
}

void migration_shutdown(void)
{
}

void precopy_infrastructure_init(void)
{
}

void postcopy_infrastructure_init(void)
{
}

void dump_vmstate_json_to_file(FILE *out_fp)
{
    (void)out_fp;
}
