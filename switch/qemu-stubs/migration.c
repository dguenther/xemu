/*
 * QEMU Stubs for Nintendo Switch - Chardev (Character Devices)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "common.h"

bool migration_in_colo_state(void)
{
    switch_log("STUB: migration_in_colo_state called\n");
    return false;
}

bool migration_incoming_in_colo_state(void)
{
    switch_log("STUB: migration_incoming_in_colo_state called\n");
    return false;
}

void coroutine_fn colo_incoming_co(void)
{
    switch_log("STUB: colo_incoming_co called\n");
}

void migrate_channel_requires_tls_upgrade(void)
{
    switch_log("STUB: migrate_channel_requires_tls_upgrade called\n");
}

void migration_tls_channel_process_incoming(void)
{
    switch_log("STUB: migration_tls_channel_process_incoming called\n");
}

void exec_start_incoming_migration(void)
{
    switch_log("STUB: exec_start_incoming_migration called\n");
}

void migration_reset_vfio_bytes_transferred(void)
{
    switch_log("STUB: migration_reset_vfio_bytes_transferred called\n");
}
