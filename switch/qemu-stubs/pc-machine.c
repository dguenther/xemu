/*
 * QEMU Stubs for Nintendo Switch - PC Machine (x86)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef struct ISADevice ISADevice;
typedef struct ISABus ISABus;
typedef struct PCIBus PCIBus;
typedef enum SmbiosEntryPointType SmbiosEntryPointType;
typedef enum OnOffAuto OnOffAuto;
typedef enum FloppyDriveType FloppyDriveType;
typedef struct BusState BusState;

unsigned int machine_topo_get_cores_per_socket(void *ms)
{
    switch_log("STUB: machine_topo_get_cores_per_socket called\n");
    (void)ms;
    return 1;
}

__attribute__((weak)) void cpu_set_ignne(void)
{
    switch_log("STUB: cpu_set_ignne called\n");
}

__attribute__((weak)) void mc146818rtc_set_cmos_data(MC146818RtcState *s,
                                                     int addr, int data)
{
    switch_log("STUB: mc146818rtc_set_cmos_data called\n");
    (void)s;
    (void)addr;
    (void)data;
}

__attribute__((weak)) uint8_t mc146818rtc_get_cmos_data(MC146818RtcState *s,
                                                        int addr)
{
    switch_log("STUB: mc146818rtc_get_cmos_data called\n");
    (void)s;
    (void)addr;
    return 0;
}

__attribute__((weak)) X86IOMMUState *x86_iommu_get_default(void)
{
    switch_log("STUB: x86_iommu_get_default called\n");
    return NULL;
}

__attribute__((weak)) void visit_type_OnOffAuto(Visitor *v, const char *name,
                                                OnOffAuto *obj, Error **errp)
{
    switch_log("STUB: visit_type_OnOffAuto called\n");
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
}

__attribute__((weak)) void visit_type_SmbiosEntryPointType(
    Visitor *v, const char *name, SmbiosEntryPointType *obj, Error **errp)
{
    switch_log("STUB: visit_type_SmbiosEntryPointType called\n");
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
}

__attribute__((weak)) void pc_system_flash_create(PCMachineState *pcms)
{
    switch_log("STUB: pc_system_flash_create called\n");
    (void)pcms;
}

__attribute__((weak)) void cxl_machine_init(Object *obj, CXLState *state)
{
    switch_log("STUB: cxl_machine_init called\n");
    (void)obj;
    (void)state;
}

__attribute__((weak)) void cxl_hook_up_pxb_registers(PCIBus *bus, CXLState *state,
                                                     Error **errp)
{
    switch_log("STUB: cxl_hook_up_pxb_registers called\n");
    (void)bus;
    (void)state;
    (void)errp;
}

__attribute__((weak)) void cxl_fmws_link_targets(CXLState *stat, Error **errp)
{
    switch_log("STUB: cxl_fmws_link_targets called\n");
    (void)stat;
    (void)errp;
}

__attribute__((weak)) void x86_rtc_set_cpus_count(ISADevice *rtc,
                                                  uint16_t cpus_count)
{
    switch_log("STUB: x86_rtc_set_cpus_count called\n");
    (void)rtc;
    (void)cpus_count;
}

__attribute__((weak)) void fw_cfg_add_extra_pci_roots(PCIBus *bus, FWCfgState *s)
{
    switch_log("STUB: fw_cfg_add_extra_pci_roots called\n");
    (void)bus;
    (void)s;
}

__attribute__((weak)) void acpi_setup(void)
{
    switch_log("STUB: acpi_setup called\n");
}

__attribute__((weak)) void fw_cfg_build_smbios(PCMachineState *pcms,
                                               FWCfgState *fw_cfg,
                                               SmbiosEntryPointType ep_type)
{
    switch_log("STUB: fw_cfg_build_smbios called\n");
    (void)pcms;
    (void)fw_cfg;
    (void)ep_type;
}

__attribute__((weak)) void fw_cfg_add_e820(FWCfgState *fw_cfg)
{
    switch_log("STUB: fw_cfg_add_e820 called\n");
    (void)fw_cfg;
}

__attribute__((weak)) void fw_cfg_build_feature_control(MachineState *ms,
                                                        FWCfgState *fw_cfg)
{
    switch_log("STUB: fw_cfg_build_feature_control called\n");
    (void)ms;
    (void)fw_cfg;
}

__attribute__((weak)) void fw_cfg_modify_i16(FWCfgState *s, uint16_t key,
                                             uint16_t value)
{
    switch_log("STUB: fw_cfg_modify_i16 called\n");
    (void)s;
    (void)key;
    (void)value;
}

__attribute__((weak)) int ide_get_geometry(BusState *bus, int unit,
                                           int16_t *cyls, int8_t *heads,
                                           int8_t *secs)
{
    switch_log("STUB: ide_get_geometry called\n");
    (void)bus;
    (void)unit;
    (void)cyls;
    (void)heads;
    (void)secs;
    return -1;
}

__attribute__((weak)) int ide_get_bios_chs_trans(BusState *bus, int unit)
{
    switch_log("STUB: ide_get_bios_chs_trans called\n");
    (void)bus;
    (void)unit;
    return 1;
}

__attribute__((weak)) int cmos_get_fd_drive_type(FloppyDriveType fd0)
{
    switch_log("STUB: cmos_get_fd_drive_type called\n");
    (void)fd0;
    return 0;
}

__attribute__((weak)) bool x86_machine_is_acpi_enabled(const X86MachineState *x86ms)
{
    switch_log("STUB: x86_machine_is_acpi_enabled called\n");
    (void)x86ms;
    return false;
}

__attribute__((weak)) void x86_cpu_pre_plug(HotplugHandler *hotplug_dev,
                                            DeviceState *dev, Error **errp)
{
    switch_log("STUB: x86_cpu_pre_plug called\n");
    (void)hotplug_dev;
    (void)dev;
    (void)errp;
}

__attribute__((weak)) void x86_cpu_plug(HotplugHandler *hotplug_dev,
                                        DeviceState *dev, Error **errp)
{
    switch_log("STUB: x86_cpu_plug called\n");
    (void)hotplug_dev;
    (void)dev;
    (void)errp;
}

__attribute__((weak)) void x86_cpu_unplug_request_cb(HotplugHandler *hotplug_dev,
                                                     DeviceState *dev,
                                                     Error **errp)
{
    switch_log("STUB: x86_cpu_unplug_request_cb called\n");
    (void)hotplug_dev;
    (void)dev;
    (void)errp;
}

__attribute__((weak)) void x86_cpu_unplug_cb(HotplugHandler *hotplug_dev,
                                             DeviceState *dev, Error **errp)
{
    switch_log("STUB: x86_cpu_unplug_cb called\n");
    (void)hotplug_dev;
    (void)dev;
    (void)errp;
}

__attribute__((weak)) void hotplug_handler_unplug_request(
    HotplugHandler *plug_handler, DeviceState *dev, Error **errp)
{
    switch_log("STUB: hotplug_handler_unplug_request called\n");
    (void)plug_handler;
    (void)dev;
    (void)errp;
}

__attribute__((weak)) void hotplug_handler_unplug(HotplugHandler *plug_handler,
                                                  DeviceState *dev,
                                                  Error **errp)
{
    switch_log("STUB: hotplug_handler_unplug called\n");
    (void)plug_handler;
    (void)dev;
    (void)errp;
}

__attribute__((weak)) void pc_dimm_pre_plug(PCDIMMDevice *dimm,
                                            MachineState *machine, Error **errp)
{
    switch_log("STUB: pc_dimm_pre_plug called\n");
    (void)dimm;
    (void)machine;
    (void)errp;
}

__attribute__((weak)) void pc_dimm_plug(PCDIMMDevice *dimm, MachineState *machine)
{
    switch_log("STUB: pc_dimm_plug called\n");
    (void)dimm;
    (void)machine;
}

__attribute__((weak)) void pc_dimm_unplug(PCDIMMDevice *dimm, MachineState *machine)
{
    switch_log("STUB: pc_dimm_unplug called\n");
    (void)dimm;
    (void)machine;
}

__attribute__((weak)) void nvdimm_plug(NVDIMMState *state)
{
    switch_log("STUB: nvdimm_plug called\n");
    (void)state;
}

__attribute__((weak)) void memory_device_pre_plug(MemoryDeviceState *md,
                                                  MachineState *ms,
                                                  Error **errp)
{
    switch_log("STUB: memory_device_pre_plug called\n");
    (void)md;
    (void)ms;
    (void)errp;
}

__attribute__((weak)) void memory_device_plug(MemoryDeviceState *md,
                                              MachineState *ms)
{
    switch_log("STUB: memory_device_plug called\n");
    (void)md;
    (void)ms;
}

__attribute__((weak)) void virtio_md_pci_pre_plug(VirtIOMDPCI *vmd,
                                                  MachineState *ms,
                                                  Error **errp)
{
    switch_log("STUB: virtio_md_pci_pre_plug called\n");
    (void)vmd;
    (void)ms;
    (void)errp;
}

__attribute__((weak)) void virtio_md_pci_plug(VirtIOMDPCI *vmd,
                                              MachineState *ms,
                                              Error **errp)
{
    switch_log("STUB: virtio_md_pci_plug called\n");
    (void)vmd;
    (void)ms;
    (void)errp;
}

__attribute__((weak)) void virtio_md_pci_unplug_request(VirtIOMDPCI *vmd,
                                                        MachineState *ms,
                                                        Error **errp)
{
    switch_log("STUB: virtio_md_pci_unplug_request called\n");
    (void)vmd;
    (void)ms;
    (void)errp;
}

__attribute__((weak)) void virtio_md_pci_unplug(VirtIOMDPCI *vmd,
                                                MachineState *ms,
                                                Error **errp)
{
    switch_log("STUB: virtio_md_pci_unplug called\n");
    (void)vmd;
    (void)ms;
    (void)errp;
}

void fw_cfg_set_order_override(FWCfgState *fw_cfg, int order)
{
    switch_log("STUB: fw_cfg_set_order_override called\n");
    (void)fw_cfg;
    (void)order;
}

void fw_cfg_reset_order_override(FWCfgState *fw_cfg)
{
    switch_log("STUB: fw_cfg_reset_order_override called\n");
    (void)fw_cfg;
}

bool fw_cfg_add_from_generator(FWCfgState *s, const char *filename,
                               const char *gen_id, Error **errp)
{
    switch_log("STUB: fw_cfg_add_from_generator called\n");
    (void)s;
    (void)filename;
    (void)gen_id;
    (void)errp;
    return false;
}

__attribute__((weak)) void qemu_init_arch_modules(void)
{
    switch_log("STUB: qemu_init_arch_modules called\n");
}

void qemu_set_hw_version(const char *version)
{
    switch_log("STUB: qemu_set_hw_version called\n");
    (void)version;
}

const char *qemu_hw_version(void)
{
    return "1.0";
}

void register_global_state(void)
{
    switch_log("STUB: register_global_state called\n");
}

void __attribute__((weak)) qapi_free_BootConfiguration(BootConfiguration *obj)
{
    switch_log("STUB: qapi_free_BootConfiguration called\n");
    (void)obj;
}

void __attribute__((weak)) qapi_free_MemorySizeConfiguration(MemorySizeConfiguration *obj)
{
    switch_log("STUB: qapi_free_MemorySizeConfiguration called\n");
    free(obj);
}
