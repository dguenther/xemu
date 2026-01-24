/*
 * QEMU Stubs for Nintendo Switch - QMP/QAPI Commands
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * QMP command stubs (block device control)
 */
void qmp_eject(const char *device, bool has_force, bool force, void *errp)
{
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
    (void)errp;
}

void qmp_migrate_incoming(const char *uri, bool has_channels,
                          MigrationChannelList *channels,
                          bool has_exit_on_error, bool exit_on_error,
                          Error **errp)
{
    (void)uri;
    (void)has_channels;
    (void)channels;
    (void)has_exit_on_error;
    (void)exit_on_error;
    (void)errp;
}

void qmp_quit(Error **errp)
{
    (void)errp;
    exit(0);
}
