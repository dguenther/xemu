/*
 * QEMU type stubs for Nintendo Switch build
 *
 * Provides minimal stub types for QEMU data structures
 * that are referenced by the UI code but not needed for HUD-only mode.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_QEMU_TYPES_STUB_H
#define SWITCH_QEMU_TYPES_STUB_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>  /* For access() */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Error type stub - matches QEMU's qapi/error.h
 */
typedef struct Error Error;

/* Error handling stubs */
static inline void error_free(Error *err) { (void)err; }
static inline bool error_is_set(Error **errp) { (void)errp; return false; }
static inline const char *error_get_pretty(const Error *err) { (void)err; return ""; }

#define error_setg(errp, fmt, ...) do { (void)(errp); } while(0)
#define error_report(fmt, ...) do { } while(0)
static inline void error_report_err(Error *err) { (void)err; }

/*
 * QEMUSnapshotInfo stub - matches block/snapshot.h
 */
#define SNAPSHOT_NAME_SIZE 256
typedef struct QEMUSnapshotInfo {
    char id_str[128];
    char name[SNAPSHOT_NAME_SIZE];
    uint64_t vm_state_size;
    uint32_t date_sec;
    uint32_t date_nsec;
    uint64_t vm_clock_nsec;
    uint64_t icount;
} QEMUSnapshotInfo;

/*
 * QEMUFile stub - for save state operations
 */
typedef struct QEMUFile QEMUFile;

/*
 * RunState enum stub
 */
typedef enum RunState {
    RUN_STATE_DEBUG,
    RUN_STATE_INMIGRATE,
    RUN_STATE_INTERNAL_ERROR,
    RUN_STATE_IO_ERROR,
    RUN_STATE_PAUSED,
    RUN_STATE_POSTMIGRATE,
    RUN_STATE_PRELAUNCH,
    RUN_STATE_FINISH_MIGRATE,
    RUN_STATE_RESTORE_VM,
    RUN_STATE_RUNNING,
    RUN_STATE_SAVE_VM,
    RUN_STATE_SHUTDOWN,
    RUN_STATE_SUSPENDED,
    RUN_STATE_WATCHDOG,
    RUN_STATE_GUEST_PANICKED,
    RUN_STATE_COLO,
    RUN_STATE__MAX,
} RunState;

/*
 * VM control function stubs
 */
static inline bool runstate_is_running(void) { return false; }
static inline int vm_stop(RunState state) { (void)state; return 0; }
static inline void vm_start(void) { }
static inline RunState runstate_get(void) { return RUN_STATE_PAUSED; }

/*
 * File operations stub
 * Guard to prevent redefinition if qemu/osdep.h is also included
 */
#ifndef qemu_fopen
static inline FILE *qemu_fopen(const char *path, const char *mode) {
    return fopen(path, mode);
}
#endif

#ifndef qemu_access
static inline int qemu_access(const char *path, int mode) {
    return access(path, mode);
}
#endif

/* F_OK may not be defined */
#ifndef F_OK
#define F_OK 0
#endif
#ifndef R_OK
#define R_OK 4
#endif
#ifndef W_OK
#define W_OK 2
#endif
#ifndef X_OK
#define X_OK 1
#endif

/*
 * GRegex stub (from GLib, but we don't have full glib regex support)
 */
typedef void GRegex;

static inline GRegex *g_regex_new(const char *pattern, int compile_options,
                                   int match_options, void *error) {
    (void)pattern; (void)compile_options; (void)match_options; (void)error;
    return NULL;
}

static inline void g_regex_unref(GRegex *regex) { (void)regex; }

static inline bool g_regex_match(GRegex *regex, const char *string,
                                  int match_options, void *match_info) {
    (void)regex; (void)string; (void)match_options; (void)match_info;
    return true;  /* Match everything when no regex */
}

#define G_REGEX_CASELESS 0
#define G_REGEX_OPTIMIZE 0

/*
 * QemuConsole stub
 */
typedef struct QemuConsole QemuConsole;
typedef struct DisplaySurface DisplaySurface;

/*
 * Additional QEMU function stubs
 */
static inline QemuConsole *qemu_console_lookup_by_index(unsigned int index) {
    (void)index;
    return NULL;
}

static inline bool qemu_console_is_graphic(QemuConsole *con) {
    (void)con;
    return true;
}

static inline DisplaySurface *qemu_console_surface(QemuConsole *con) {
    (void)con;
    return NULL;
}

static inline int surface_width(DisplaySurface *s) {
    (void)s;
    return 640;
}

static inline int surface_height(DisplaySurface *s) {
    (void)s;
    return 480;
}

/*
 * NV2A GPU stubs
 */
static inline int nv2a_get_surface_scale_factor(void) { return 1; }
static inline void nv2a_set_surface_scale_factor(int factor) { (void)factor; }
static inline const uint8_t *nv2a_get_dac_palette(void) { return NULL; }
static inline bool nv2a_get_screen_off(void) { return false; }

/*
 * QTAILQ stub macros
 */
#ifndef QTAILQ_FOREACH
#define QTAILQ_FOREACH(var, head, field) \
    for ((var) = ((head)->tqh_first); \
         (var); \
         (var) = ((var)->field.tqe_next))
#endif

#ifndef QTAILQ_HEAD
#define QTAILQ_HEAD(name, type) \
    struct name { \
        struct type *tqh_first; \
        struct type **tqh_last; \
    }
#endif

#ifndef QTAILQ_ENTRY
#define QTAILQ_ENTRY(type) \
    struct { \
        struct type *tqe_next; \
        struct type **tqe_prev; \
    }
#endif

/*
 * GMatchInfo stub
 */
typedef void GMatchInfo;

static inline bool g_match_info_matches(GMatchInfo *match_info) {
    (void)match_info;
    return true;
}

static inline void g_match_info_free(GMatchInfo *match_info) {
    (void)match_info;
}

typedef int GRegexCompileFlags;
typedef int GRegexMatchFlags;

/*
 * Additional GDateTime functions
 */
typedef struct _GDateTime GDateTime;
typedef struct _GTimeZone GTimeZone;

static inline GDateTime *g_date_time_new_from_unix_local(int64_t t) {
    (void)t;
    return NULL;
}

/*
 * g_utf16_to_utf8 stub
 */
static inline char *g_utf16_to_utf8(const uint16_t *str, long len,
                                     long *items_read, long *items_written,
                                     void *error) {
    (void)str; (void)len; (void)items_read; (void)items_written; (void)error;
    return NULL;
}

/*
 * pcap constants
 */
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

/*
 * Network stubs
 */
static inline bool xemu_net_is_enabled(void) { return false; }
static inline void xemu_net_enable(void) { }
static inline void xemu_net_disable(void) { }

/*
 * Disc/system control stubs
 */
static inline void xemu_eject_disc(Error **err) { (void)err; }
static inline void xemu_load_disc(const char *path, Error **err) {
    (void)path; (void)err;
}

typedef enum ShutdownCause {
    SHUTDOWN_CAUSE_NONE,
    SHUTDOWN_CAUSE_HOST_ERROR,
    SHUTDOWN_CAUSE_HOST_QMP_QUIT,
    SHUTDOWN_CAUSE_HOST_QMP_SYSTEM_RESET,
    SHUTDOWN_CAUSE_HOST_SIGNAL,
    SHUTDOWN_CAUSE_HOST_UI,
    SHUTDOWN_CAUSE_GUEST_SHUTDOWN,
    SHUTDOWN_CAUSE_GUEST_RESET,
    SHUTDOWN_CAUSE_GUEST_PANIC,
    SHUTDOWN_CAUSE__MAX,
} ShutdownCause;

static inline void qemu_system_reset_request(ShutdownCause cause) { (void)cause; }
static inline void qemu_system_shutdown_request(ShutdownCause cause) { (void)cause; }

/*
 * FATX image stub
 */
static inline bool create_fatx_image(const char *path, size_t size) {
    (void)path;
    (void)size;
    return false;
}

#ifdef __cplusplus
}
#endif

#endif /* SWITCH_QEMU_TYPES_STUB_H */
