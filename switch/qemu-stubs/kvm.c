/*
 * QEMU Stubs for Nintendo Switch
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "common.h"

void kvm_dirty_ring_enabled(void)
{
    switch_log("STUB: kvm_dirty_ring_enabled called\n");
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