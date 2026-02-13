/*
 * QEMU Stubs for Nintendo Switch - Xen helpers
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void xen_register_framebuffer(MemoryRegion *mr)
{
    switch_log("STUB: xen_register_framebuffer called\n");
    (void)mr;
}

void xen_evtchn_snoop_msi()
{
    switch_log("STUB: xen_evtchn_snoop_msi called\n");
}

void xen_evtchn_remove_pci_device()
{
    switch_log("STUB: xen_evtchn_snoop_msi called\n");
}

void xen_evtchn_deliver_pirq_msi()
{
    switch_log("STUB: xen_evtchn_snoop_msi called\n");
}

void qmp_xen_set_global_dirty_log()
{
    switch_log("STUB: qmp_xen_set_global_dirty_log called\n");
}

void qmp_xen_event_list()
{
    switch_log("STUB: qmp_xen_event_list called\n");
}

void qmp_xen_event_inject()
{
    switch_log("STUB: qmp_xen_event_inject called\n");
}