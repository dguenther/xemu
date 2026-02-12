/*
 * QEMU Stubs for Nintendo Switch - QMP/QAPI Commands
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

extern void vm_start(void);

/*
 * QMP command stubs (block device control)
 */
void qmp_eject(const char *device, bool has_force, bool force, void *errp)
{
    switch_log("STUB: qmp_eject called\n");
    (void)device;
    (void)has_force;
    (void)force;
    (void)errp;
}

void qmp_blockdev_change_medium(const char *device,
                                 const char *filename,
                                 bool has_format, const char *format,
                                 bool has_read_only, int read_only_mode,
                                 void *errp)
{
    switch_log("STUB: qmp_blockdev_change_medium called\n");
    (void)device;
    (void)filename;
    (void)has_format;
    (void)format;
    (void)has_read_only;
    (void)read_only_mode;
    (void)errp;
}

void qmp_cont(Error **errp)
{
    switch_log("STUB: qmp_cont called\n");
    (void)errp;
    vm_start();
}

void qmp_migrate_incoming(const char *uri, bool has_channels,
                          MigrationChannelList *channels,
                          bool has_exit_on_error, bool exit_on_error,
                          Error **errp)
{
    switch_log("STUB: qmp_migrate_incoming called\n");
    (void)uri;
    (void)has_channels;
    (void)channels;
    (void)has_exit_on_error;
    (void)exit_on_error;
    (void)errp;
}

void qmp_quit(Error **errp)
{
    switch_log("STUB: qmp_quit called\n");
    (void)errp;
    exit(0);
}
