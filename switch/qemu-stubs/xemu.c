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
void xemu_input_set_test_mode(int mode) { switch_log("STUB: xemu_input_set_test_mode called\n"); (void)mode; }
void xemu_input_process_sdl_events(void *event) { switch_log("STUB: xemu_input_process_sdl_events called\n"); (void)event; }
void xemu_input_update_controllers(void) { switch_log("STUB: xemu_input_update_controllers called\n"); }
void xemu_snapshots_set_framebuffer_texture(unsigned int tex, bool flip)
{
    switch_log("STUB: xemu_snapshots_set_framebuffer_texture called\n");
    (void)tex;
    (void)flip;
}

/*
 * Controller list stub
 */
struct ControllerStateList {
    void *tqh_first;
    void **tqh_last;
};
struct ControllerStateList available_controllers = { NULL, NULL };

void xemu_snapshots_delete(const char *name, void *err) { switch_log("STUB: xemu_snapshots_delete called\n"); (void)name; (void)err; }

/* Input binding stubs */
void *xemu_input_get_bound(int port) { switch_log("STUB: xemu_input_get_bound called\n"); (void)port; return NULL; }
void xemu_input_bind(int port, void *state, int type) { switch_log("STUB: xemu_input_bind called\n"); (void)port; (void)state; (void)type; }
void xemu_input_bind_xmu(int port, const char *path) { switch_log("STUB: xemu_input_bind_xmu called\n"); (void)port; (void)path; }
void xemu_input_unbind_xmu(int port) { switch_log("STUB: xemu_input_unbind_xmu called\n"); (void)port; }
void xemu_input_rebind_xmu(int port) { switch_log("STUB: xemu_input_rebind_xmu called\n"); (void)port; }
void xemu_input_reset_input_mapping(void) { switch_log("STUB: xemu_input_reset_input_mapping called\n"); }
void xemu_save_peripheral_settings(int port, void *state) { switch_log("STUB: xemu_save_peripheral_settings called\n"); (void)port; (void)state; }

/* Bound controllers array */
void *bound_controllers[4] = { NULL, NULL, NULL, NULL };

/* Keyboard scancode map */
const char *g_keyboard_scancode_map[256] = { NULL };

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

bool xbox_eeprom_generate(const char *file, int ver)
{
    switch_log("STUB: xbox_eeprom_generate called\n");
    (void)file;
    (void)ver;
    return false;
}
