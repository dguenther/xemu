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

void qobject_destroy(QObject *obj)
{
    switch_log("STUB: qobject_destroy called\n");
    /* Stub - actual cleanup would be done here */
    (void)obj;
}

bool qobject_is_equal(const QObject *x, const QObject *y)
{
    switch_log("STUB: qobject_is_equal called\n");
    return x == y;
}

QDict *qmp_event_build_dict(const char *event_name)
{
    switch_log("STUB: qmp_event_build_dict called\n");
    (void)event_name;
    return NULL;
}

Visitor *qobject_output_visitor_new_qmp(QObject **result)
{
    switch_log("STUB: qobject_output_visitor_new_qmp called\n");
    (void)result;
    return NULL;
}

void qapi_event_emit(int event, QDict *qdict)
{
    switch_log("STUB: qapi_event_emit called\n");
    (void)event;
    (void)qdict;
}

const QEnumLookup ObjectType_lookup = {
    .array = NULL,
    .special_features = NULL,
    .size = 0,
};

bool visit_type_ChardevBackend(Visitor *v, const char *name, void **obj, Error **errp)
{
    switch_log("STUB: visit_type_ChardevBackend called\n");
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

bool visit_type_ObjectOptions(Visitor *v, const char *name,
                              ObjectOptions **obj, Error **errp)
{
    switch_log("STUB: visit_type_ObjectOptions called\n");
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

void qapi_free_ObjectOptions(ObjectOptions *obj)
{
    switch_log("STUB: qapi_free_ObjectOptions called\n");
    (void)obj;
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

bool visit_type_AcpiTableOptions(Visitor *v, const char *name,
                                 void *obj, Error **errp)
{
    switch_log("STUB: visit_type_AcpiTableOptions called\n");
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

void qapi_free_AcpiTableOptions(void *obj)
{
    switch_log("STUB: qapi_free_AcpiTableOptions called\n");
    (void)obj;
}

void qmp_marshal_set_action(QDict *args, QObject **ret, Error **errp)
{
    switch_log("STUB: qmp_marshal_set_action called\n");
    (void)args;
    (void)ret;
    (void)errp;
}

void qmp_device_add(QDict *qdict, QObject **ret_data, Error **errp)
{
    switch_log("STUB: qmp_device_add called\n");
    (void)qdict;
    (void)ret_data;
    (void)errp;
}
