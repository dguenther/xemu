/*
 * QEMU Stubs for Nintendo Switch - PCI Bus
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef struct PCIDevice PCIDevice;
typedef struct MemoryRegion MemoryRegion;

/* Keep MSI paths enabled in APIC realize for Switch bring-up. */
bool msi_nonbroken = true;

/*
 * PCI helpers (stubbed for Switch bring-up)
 */
__attribute__((weak)) void pci_register_bar(PCIDevice *pci_dev, int region_num,
                                             uint8_t attr, MemoryRegion *memory)
{
    switch_log("STUB: pci_register_bar called\n");
    (void)pci_dev;
    (void)region_num;
    (void)attr;
    (void)memory;
}

uint8_t pcie_cap_get_type(const PCIDevice *dev)
{
    switch_log("STUB: pcie_cap_get_type called\n");
    (void)dev;
    return 0;
}

void pcie_sync_bridge_lnk(PCIDevice *bridge_dev)
{
    switch_log("STUB: pcie_sync_bridge_lnk called\n");
    (void)bridge_dev;
}

uint16_t pcie_find_capability(PCIDevice *dev, uint16_t cap_id)
{
    switch_log("STUB: pcie_find_capability called\n");
    (void)dev;
    (void)cap_id;
    return 0;
}

void pcie_sriov_pf_reset(PCIDevice *dev)
{
    switch_log("STUB: pcie_sriov_pf_reset called\n");
    (void)dev;
}

void pcie_sriov_config_write(PCIDevice *dev, uint32_t address,
                             uint32_t val, int len)
{
    switch_log("STUB: pcie_sriov_config_write called\n");
    (void)dev;
    (void)address;
    (void)val;
    (void)len;
}

void msi_reset(PCIDevice *dev)
{
    switch_log("STUB: msi_reset called\n");
    (void)dev;
}

void msix_reset(PCIDevice *dev)
{
    switch_log("STUB: msix_reset called\n");
    (void)dev;
}

void msi_write_config(PCIDevice *dev, uint32_t address, uint32_t val, int len)
{
    switch_log("STUB: msi_write_config called\n");
    (void)dev;
    (void)address;
    (void)val;
    (void)len;
}

void msix_write_config(PCIDevice *dev, uint32_t address, uint32_t val, int len)
{
    switch_log("STUB: msix_write_config called\n");
    (void)dev;
    (void)address;
    (void)val;
    (void)len;
}

void xen_evtchn_remove_pci_device(PCIDevice *dev)
{
    switch_log("STUB: xen_evtchn_remove_pci_device called\n");
    (void)dev;
}

void xen_evtchn_deliver_pirq_msi(PCIDevice *dev)
{
    switch_log("STUB: xen_evtchn_deliver_pirq_msi called\n");
    (void)dev;
}

__attribute__((weak)) const MemoryRegionOps pci_host_conf_le_ops = { 0 };
__attribute__((weak)) const MemoryRegionOps pci_host_data_le_ops = { 0 };

void build_pci_bridge_aml(AcpiDevAmlIf *adev, Aml *scope)
{
    switch_log("STUB: build_pci_bridge_aml called\n");
    (void)adev;
    (void)scope;
}

typedef struct ISABus ISABus;
typedef struct ISADevice ISADevice;

/* Note: x86_allocate_cpu_irq and i8259_init are now in QEMU core */

__attribute__((weak)) void gsi_handler(void *opaque, int n, int level)
{
    switch_log("STUB: gsi_handler called\n");
    (void)opaque;
    (void)n;
    (void)level;
}
