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
__attribute__((weak)) const VMStateInfo vmstate_info_uint8_equal = { 0 };
extern const void *vmstate_info_buffer;
const void *vmstate_info_buffer = NULL;

static int vmstate_stub_get(QEMUFile *f, void *pv, size_t size,
                            const VMStateField *field)
{
    switch_log("STUB: vmstate_stub_get called\n");
    (void)f;
    (void)pv;
    (void)size;
    (void)field;
    return 0;
}

static int vmstate_stub_put(QEMUFile *f, void *pv, size_t size,
                            const VMStateField *field, JSONWriter *vmdesc)
{
    switch_log("STUB: vmstate_stub_put called\n");
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
    switch_log("STUB: vmstate_check_only_migratable called\n");
    (void)vmsd;
    return true;
}

void vmstate_register_ram_global(MemoryRegion *mr)
{
    switch_log("STUB: vmstate_register_ram_global called\n");
    (void)mr;
}

void vmstate_unregister_ram(MemoryRegion *mr, DeviceState *dev)
{
    switch_log("STUB: vmstate_unregister_ram called\n");
    (void)mr;
    (void)dev;
}

void qemu_put_buffer(QEMUFile *f, const uint8_t *buf, size_t size)
{
    switch_log("STUB: qemu_put_buffer called\n");
    (void)f;
    (void)buf;
    (void)size;
}

size_t qemu_get_buffer(QEMUFile *f, uint8_t *buf, size_t size)
{
    switch_log("STUB: qemu_get_buffer called\n");
    (void)f;
    if (buf && size) {
        memset(buf, 0, size);
    }
    return size;
}

void qemu_put_be32(QEMUFile *f, unsigned int v)
{
    switch_log("STUB: qemu_put_be32 called\n");
    (void)f;
    (void)v;
}

unsigned int qemu_get_be32(QEMUFile *f)
{
    switch_log("STUB: qemu_get_be32 called\n");
    (void)f;
    return 0;
}

bool load_snapshot(const char *name, const char *vmstate,
                   bool has_devices, strList *devices,
                   Error **errp)
{
    switch_log("STUB: load_snapshot called\n");
    (void)name;
    (void)vmstate;
    (void)has_devices;
    (void)devices;
    (void)errp;
    return false;
}

void load_snapshot_resume(int state)
{
    switch_log("STUB: load_snapshot_resume called\n");
    (void)state;
}

void migration_object_init(void)
{
    switch_log("STUB: migration_object_init called\n");
}

void migration_shutdown(void)
{
    switch_log("STUB: migration_shutdown called\n");
}

void precopy_infrastructure_init(void)
{
    switch_log("STUB: precopy_infrastructure_init called\n");
}

void postcopy_infrastructure_init(void)
{
    switch_log("STUB: postcopy_infrastructure_init called\n");
}

void dump_vmstate_json_to_file(FILE *out_fp)
{
    switch_log("STUB: dump_vmstate_json_to_file called\n");
    (void)out_fp;
}
