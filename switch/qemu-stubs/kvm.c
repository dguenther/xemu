/*
 * QEMU Stubs for Nintendo Switch
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "common.h"

bool kvm_dirty_ring_enabled(void)
{
    switch_log("STUB: kvm_dirty_ring_enabled called\n");
    return false;
}

uint32_t kvm_dirty_ring_size(void)
{
    return 0;
}

bool kvm_hwpoisoned_mem(void)
{
    return false;
}

void qmp_query_sev_attestation_report(void)
{
    switch_log("STUB: qmp_query_sev_attestation_report called\n");
}

void qmp_sev_inject_launch_secret(void)
{
    switch_log("STUB: qmp_sev_inject_launch_secret called\n");
}

void qmp_query_sev_capabilities(void)
{
    switch_log("STUB: qmp_query_sev_capabilities called\n");
}

void qmp_query_sev_launch_measure(void)
{
    switch_log("STUB: qmp_query_sev_launch_measure called\n");
}

void qmp_query_sev(void)
{
    switch_log("STUB: qmp_query_sev called\n");
}