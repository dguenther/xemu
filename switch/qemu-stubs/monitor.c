/*
 * QEMU Stubs for Nintendo Switch - Monitor
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Monitor stubs
 */

void *monitor_cur(void)
{
    switch_log("STUB: monitor_cur called\n");
    return NULL;
}

int monitor_printf(Monitor *mon, const char *fmt, ...)
{
    switch_log("STUB: monitor_printf called\n");
    (void)mon;
    (void)fmt;
    return 0;
}

int monitor_vprintf(Monitor *mon, const char *fmt, va_list ap)
{
    switch_log("STUB: monitor_vprintf called\n");
    (void)mon;
    (void)fmt;
    (void)ap;
    return 0;
}

int monitor_init_opts(QemuOpts *opts, Error **errp)
{
    switch_log("STUB: monitor_init_opts called\n");
    (void)opts;
    (void)errp;
    return 0;
}

void monitor_init_hmp(const char *chardev_name, bool use_readline, Error **errp)
{
    switch_log("STUB: monitor_init_hmp called\n");
    (void)chardev_name;
    (void)use_readline;
    (void)errp;
}

void monitor_init_globals(void)
{
    switch_log("STUB: monitor_init_globals called\n");
}

void monitor_cleanup(void)
{
    switch_log("STUB: monitor_cleanup called\n");
}

typedef struct AddfdInfo AddfdInfo;
AddfdInfo *monitor_fdset_add_fd(int fd, bool has_fdset_id, int64_t fdset_id,
                                const char *opaque, Error **errp)
{
    switch_log("STUB: monitor_fdset_add_fd called\n");
    (void)fd;
    (void)has_fdset_id;
    (void)fdset_id;
    (void)opaque;
    (void)errp;
    return NULL;
}

void pcibus_dev_print(Monitor *mon, DeviceState *dev, int indent)
{
    switch_log("STUB: pcibus_dev_print called\n");
    (void)mon;
    (void)dev;
    (void)indent;
}
