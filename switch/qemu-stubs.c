/*
 * QEMU Stubs for Nintendo Switch
 *
 * This file provides stub implementations for various QEMU functions
 * that are not available or not needed on the Switch platform.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

/*
 * Trace system stubs
 * These are used by the QEMU tracing infrastructure
 */

uint16_t trace_events_enabled_count = 0;

/* Trace state variables for mutex tracing */
uint8_t _TRACE_QEMU_MUTEX_LOCK_DSTATE = 0;
uint8_t _TRACE_QEMU_MUTEX_LOCKED_DSTATE = 0;
uint8_t _TRACE_QEMU_MUTEX_UNLOCK_DSTATE = 0;

/*
 * Big QEMU Lock (BQL) stubs
 * The BQL is used to serialize access to QEMU's global state
 */

static pthread_mutex_t bql_mutex = PTHREAD_MUTEX_INITIALIZER;

void bql_lock_impl(const char *file, int line)
{
    (void)file;
    (void)line;
    pthread_mutex_lock(&bql_mutex);
}

void bql_unlock(void)
{
    pthread_mutex_unlock(&bql_mutex);
}

bool bql_locked(void)
{
    /* Try to lock - if it succeeds, we didn't hold it */
    if (pthread_mutex_trylock(&bql_mutex) == 0) {
        pthread_mutex_unlock(&bql_mutex);
        return false;
    }
    return true;
}

/*
 * Error reporting stubs
 * Note: error_printf is already provided by util/error-report.c
 */

void error_vprintf(const char *fmt, va_list ap)
{
    vfprintf(stderr, fmt, ap);
}

/*
 * Monitor stubs
 */

void *monitor_cur(void)
{
    return NULL;
}

/*
 * QAPI visitor stubs
 * These are used by the QOM (QEMU Object Model) system
 */

typedef struct Visitor Visitor;
typedef struct Error Error;

bool visit_type_str(Visitor *v, const char *name, char **obj, Error **errp)
{
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

/*
 * QObject stubs
 */

typedef struct QObject QObject;

void qobject_destroy(QObject *obj)
{
    /* Stub - actual cleanup would be done here */
    (void)obj;
}

/*
 * File locking stubs
 * These are POSIX functions not available in newlib
 */

void flockfile(FILE *file)
{
    (void)file;
    /* No-op on Switch - single-threaded file access assumed */
}

void funlockfile(FILE *file)
{
    (void)file;
}

int ftrylockfile(FILE *file)
{
    (void)file;
    return 0; /* Success */
}

/*
 * sysconf stub
 * Returns system configuration values
 */

long sysconf(int name)
{
    /* Common sysconf values */
    switch (name) {
    case 30: /* _SC_PAGESIZE / _SC_PAGE_SIZE */
        return 4096;
    case 84: /* _SC_LEVEL1_DCACHE_SIZE */
        return 32768; /* 32KB L1 data cache for Cortex-A57 */
    case 85: /* _SC_LEVEL1_DCACHE_ASSOC */
        return 2;
    case 86: /* _SC_LEVEL1_DCACHE_LINESIZE */
        return 64;
    case 87: /* _SC_LEVEL1_ICACHE_SIZE */
        return 49152; /* 48KB L1 instruction cache for Cortex-A57 */
    case 88: /* _SC_LEVEL1_ICACHE_ASSOC */
        return 3;
    case 89: /* _SC_LEVEL1_ICACHE_LINESIZE */
        return 64;
    case 90: /* _SC_LEVEL2_CACHE_SIZE */
        return 2097152; /* 2MB L2 cache */
    case 91: /* _SC_LEVEL2_CACHE_ASSOC */
        return 16;
    case 92: /* _SC_LEVEL2_CACHE_LINESIZE */
        return 64;
    default:
        return -1;
    }
}

/*
 * pthread_sigmask stub
 * Signals are not fully supported on Switch
 * Note: newlib declares pthread_sigmask but may not implement it
 */

#include <signal.h>

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset)
{
    (void)how;
    (void)set;
    (void)oldset;
    return 0; /* Success */
}

/*
 * Data assets stubs
 * These would normally be embedded PNG data for UI assets
 */

unsigned char controller_mask_data[] = { 0 };
unsigned int controller_mask_size = 0;

unsigned char controller_mask_s_data[] = { 0 };
unsigned int controller_mask_s_size = 0;

unsigned char xmu_mask_data[] = { 0 };
unsigned int xmu_mask_size = 0;

unsigned char xemu_64x64_data[] = { 0 };
unsigned int xemu_64x64_size = 0;

/*
 * xemu-specific function stubs
 */

/* Controller binding */
int bound_drivers[4] = { 0, 0, 0, 0 };

/* Fullscreen state */
bool xemu_is_fullscreen(void) { return false; }
void xemu_toggle_fullscreen(void) { }

/* Widescreen */
bool xemu_get_widescreen(void) { return false; }

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

/*
 * POSIX process stubs
 * waitpid and execvp are not available on Switch
 */

#include <sys/types.h>

int waitpid(int pid, int *status, int options)
{
    (void)pid;
    (void)status;
    (void)options;
    return -1;  /* Always fail - no process support */
}

int execvp(const char *file, char *const argv[])
{
    (void)file;
    (void)argv;
    return -1;  /* Always fail - no process support */
}

/*
 * More font/resource data stubs
 */

unsigned char Roboto_Medium_data[] = { 0 };
unsigned int Roboto_Medium_size = 0;

unsigned char RobotoCondensed_Regular_data[] = { 0 };
unsigned int RobotoCondensed_Regular_size = 0;

unsigned char abxy_data[] = { 0 };
unsigned int abxy_size = 0;

unsigned char font_awesome_6_1_1_solid_min_data[] = { 0 };
unsigned int font_awesome_6_1_1_solid_min_size = 0;

/*
 * More xemu function stubs
 */

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

/* Note: C++ class stubs (DebugApuWindow, DebugVideoWindow, CompatibilityReporter,
 * ControllerGamepadRebindingMap, ControllerKeyboardRebindingMap) are in cpp-stubs.cc
 */

/*
 * More xemu function stubs
 */
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
