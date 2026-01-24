/*
 * QEMU Stubs for Nintendo Switch - QemuOpts Lists
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/* QemuOpts lists used during CLI init */
QemuOptsList qemu_drive_opts = {
    .name = "drive",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_drive_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_legacy_drive_opts = {
    .name = "drive-legacy",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_legacy_drive_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_common_drive_opts = {
    .name = "drive-common",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_common_drive_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_device_opts = {
    .name = "device",
    .implied_opt_name = "driver",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_device_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_netdev_opts = {
    .name = "netdev",
    .implied_opt_name = "type",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_netdev_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_nic_opts = {
    .name = "nic",
    .implied_opt_name = "type",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_nic_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_net_opts = {
    .name = "net",
    .implied_opt_name = "type",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_net_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_global_opts = {
    .name = "global",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_global_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_mon_opts = {
    .name = "mon",
    .implied_opt_name = "chardev",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_mon_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_trace_opts = {
    .name = "trace",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_trace_opts.head),
    .desc = { { NULL } },
};
QemuOptsList qemu_numa_opts = {
    .name = "numa",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_numa_opts.head),
    .desc = { { NULL } },
};

int qemu_global_option(const char *str)
{
    (void)str;
    return 0;
}
