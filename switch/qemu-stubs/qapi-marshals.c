/*
 * QEMU Stubs for Nintendo Switch - Missing QAPI marshal entrypoints
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

void qmp_marshal_dump_guest_memory(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_dump(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_dump_guest_memory_capability(QDict *args, QObject **ret,
                                                     Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_set_link(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_netdev_add(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_netdev_del(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_rx_filter(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_announce_self(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_rocker(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_rocker_ports(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_rocker_of_dpa_flows(QDict *args, QObject **ret,
                                           Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_rocker_of_dpa_groups(QDict *args, QObject **ret,
                                            Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_trace_event_get_state(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_trace_event_set_state(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}

void qmp_marshal_query_qmp_schema(QDict *args, QObject **ret, Error **errp)
{
    (void)args; (void)ret; (void)errp;
}
