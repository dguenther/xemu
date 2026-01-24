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
    /* Stub - actual cleanup would be done here */
    (void)obj;
}

bool qobject_is_equal(const QObject *x, const QObject *y)
{
    return x == y;
}

QDict *qmp_event_build_dict(const char *event_name)
{
    (void)event_name;
    return NULL;
}

Visitor *qobject_output_visitor_new_qmp(QObject **result)
{
    (void)result;
    return NULL;
}

void qapi_event_emit(int event, QDict *qdict)
{
    (void)event;
    (void)qdict;
}

bool visit_type_q_obj_SHUTDOWN_arg_members(Visitor *v, q_obj_SHUTDOWN_arg *obj,
                                           Error **errp)
{
    (void)v;
    (void)obj;
    (void)errp;
    return true;
}

bool visit_type_q_obj_RESET_arg_members(Visitor *v, q_obj_RESET_arg *obj,
                                        Error **errp)
{
    (void)v;
    (void)obj;
    (void)errp;
    return true;
}

const QEnumLookup ObjectType_lookup = {
    .array = NULL,
    .special_features = NULL,
    .size = 0,
};

bool visit_type_ChardevBackend(Visitor *v, const char *name, void **obj, Error **errp)
{
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

bool visit_type_ObjectOptions(Visitor *v, const char *name,
                              ObjectOptions **obj, Error **errp)
{
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

void qapi_free_ObjectOptions(ObjectOptions *obj)
{
    (void)obj;
}

bool visit_type_CompatPolicy_members(Visitor *v, CompatPolicy *obj,
                                     Error **errp)
{
    (void)v;
    (void)obj;
    (void)errp;
    return true;
}

bool visit_type_CompatPolicy(Visitor *v, const char *name,
                             CompatPolicy **obj, Error **errp)
{
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

void qapi_free_CompatPolicy(CompatPolicy *obj)
{
    (void)obj;
}

bool visit_type_AcpiTableOptions(Visitor *v, const char *name,
                                 void *obj, Error **errp)
{
    (void)v;
    (void)name;
    (void)obj;
    (void)errp;
    return true;
}

void qapi_free_AcpiTableOptions(void *obj)
{
    (void)obj;
}

void qmp_marshal_set_action(QDict *args, QObject **ret, Error **errp)
{
    (void)args;
    (void)ret;
    (void)errp;
}

void qmp_device_add(QDict *qdict, QObject **ret_data, Error **errp)
{
    (void)qdict;
    (void)ret_data;
    (void)errp;
}
