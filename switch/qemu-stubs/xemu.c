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
    (void)name;
    (void)err;
}

/* Debug windows */
bool apu_window = false;
bool video_window = false;
bool compatibility_reporter_window = false;

void xemu_monitor_init(void) { }
void xemu_run_monitor_command(const char *cmd) { (void)cmd; }
char *xemu_get_monitor_buffer(void) { return NULL; }

void xemu_snapshots_mark_dirty(void) { }
int xemu_snapshots_list(void **info, void **extra_data, void **err)
{
    (void)info;
    (void)extra_data;
    (void)err;
    return 0;
}

void xemu_snapshots_load(const char *name, void *err)
{
    (void)name;
    (void)err;
}

char *xemu_get_currently_loaded_disc_path(void) { return NULL; }
void xemu_input_set_test_mode(int mode) { (void)mode; }

/*
 * Controller list stub
 */
struct ControllerStateList {
    void *tqh_first;
    void **tqh_last;
};
struct ControllerStateList available_controllers = { NULL, NULL };

void xemu_snapshots_delete(const char *name, void *err) { (void)name; (void)err; }

/* Input binding stubs */
void *xemu_input_get_bound(int port) { (void)port; return NULL; }
void xemu_input_bind(int port, void *state, int type) { (void)port; (void)state; (void)type; }
void xemu_input_bind_xmu(int port, const char *path) { (void)port; (void)path; }
void xemu_input_unbind_xmu(int port) { (void)port; }
void xemu_input_rebind_xmu(int port) { (void)port; }
void xemu_input_reset_input_mapping(void) { }
void xemu_save_peripheral_settings(int port, void *state) { (void)port; (void)state; }

/* Bound controllers array */
void *bound_controllers[4] = { NULL, NULL, NULL, NULL };

/* Keyboard scancode map */
const char *g_keyboard_scancode_map[256] = { NULL };

/* XBE info */
void *xemu_get_xbe_info(void) { return NULL; }

/* OS info */
const char *xemu_get_os_info(void) { return "Nintendo Switch"; }

/* Version info */
const char *xemu_version = "0.0.0-switch";
const char *xemu_commit = "unknown";
const char *xemu_date = "unknown";

void xemu_net_enable(void)
{
}

void xemu_net_disable(void)
{
}

bool xbox_eeprom_generate(const char *file, int ver)
{
    (void)file;
    (void)ver;
    return false;
}
