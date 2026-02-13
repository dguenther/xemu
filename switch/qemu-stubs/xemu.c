/*
 * QEMU Stubs for Nintendo Switch - xemu-specific Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * xemu-specific function stubs
 */

/* Snapshot shortcuts map */
const char *g_snapshot_shortcut_index_key_map_storage[4] = { NULL, NULL, NULL, NULL };
const char **g_snapshot_shortcut_index_key_map = g_snapshot_shortcut_index_key_map_storage;

/* Snapshot operations */
void xemu_snapshots_save(const char *name, void *err)
{
    switch_log("STUB: xemu_snapshots_save called\n");
    (void)name;
    (void)err;
}

/* Debug windows */
bool apu_window = false;
bool video_window = false;
bool compatibility_reporter_window = false;

void xemu_monitor_init(void) { switch_log("STUB: xemu_monitor_init called\n"); }
void xemu_run_monitor_command(const char *cmd) { switch_log("STUB: xemu_run_monitor_command called\n"); (void)cmd; }
char *xemu_get_monitor_buffer(void) { switch_log("STUB: xemu_get_monitor_buffer called\n"); return NULL; }

void xemu_snapshots_mark_dirty(void) { switch_log("STUB: xemu_snapshots_mark_dirty called\n"); }
int xemu_snapshots_list(void **info, void **extra_data, void **err)
{
    switch_log("STUB: xemu_snapshots_list called\n");
    (void)info;
    (void)extra_data;
    (void)err;
    return 0;
}

void xemu_snapshots_load(const char *name, void *err)
{
    switch_log("STUB: xemu_snapshots_load called\n");
    (void)name;
    (void)err;
}

char *xemu_get_currently_loaded_disc_path(void) { switch_log("STUB: xemu_get_currently_loaded_disc_path called\n"); return NULL; }

/*
 * Controller list stub
 */
struct ControllerStateList {
    void *tqh_first;
    void **tqh_last;
};

void xemu_snapshots_delete(const char *name, void *err) { switch_log("STUB: xemu_snapshots_delete called\n"); (void)name; (void)err; }

/* XBE info */
void *xemu_get_xbe_info(void) { switch_log("STUB: xemu_get_xbe_info called\n"); return NULL; }

/* OS info */
const char *xemu_get_os_info(void) { return "Nintendo Switch"; }

/* Version info */
const char *xemu_version = "0.0.0-switch";
const char *xemu_commit = "unknown";
const char *xemu_date = "unknown";

void xemu_net_enable(void)
{
    switch_log("STUB: xemu_net_enable called\n");
}

void xemu_net_disable(void)
{
    switch_log("STUB: xemu_net_disable called\n");
}

void xemu_net_is_enabled(void)
{
    switch_log("STUB: xemu_net_is_enabled called\n");
}
