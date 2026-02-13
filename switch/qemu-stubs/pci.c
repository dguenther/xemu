/*
 * QEMU Stubs for Nintendo Switch - PCI Bus
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef struct PCIDevice PCIDevice;
typedef struct MemoryRegion MemoryRegion;

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
