/*
 * QEMU Stubs for Nintendo Switch - QObject/QAPI
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * QObject stubs
 */

typedef struct QObject QObject;

QDict *qmp_event_build_dict(const char *event_name)
{
    switch_log("STUB: qmp_event_build_dict called\n");
    (void)event_name;
    return NULL;
}

bool visit_type_CompatPolicy_members(Visitor *v, CompatPolicy *obj,
                                     Error **errp)
{
    switch_log("STUB: visit_type_CompatPolicy_members called\n");
    (void)v;
    (void)obj;
    (void)errp;
    return true;
}

bool visit_type_CompatPolicy(Visitor *v, const char *name,
                             CompatPolicy **obj, Error **errp)
{
    switch_log("STUB: visit_type_CompatPolicy called\n");
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

void qapi_free_CompatPolicy(CompatPolicy *obj)
{
    switch_log("STUB: qapi_free_CompatPolicy called\n");
    (void)obj;
}

void qmp_marshal_set_action(QDict *args, QObject **ret, Error **errp)
{
    switch_log("STUB: qmp_marshal_set_action called\n");
    (void)args;
    (void)ret;
    (void)errp;
}