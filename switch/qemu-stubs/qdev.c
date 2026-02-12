/*
 * QEMU Stubs for Nintendo Switch - QDev Device Model
 *
 * Note: These stubs are needed even with the full QEMU core because
 * the source files that implement these (hw/core/qdev-properties.c)
 * rely on C++ name mangling differences.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/* Property and PropertyInfo are now defined in hw/qdev-properties.h */
void qdev_propinfo_get_int32(Object *obj, Visitor *v, const char *name,
                             void *opaque, Error **errp);
void qdev_propinfo_set_default_value_int(ObjectProperty *op,
                                         const Property *prop);

/* Stub property get/set functions */
static void stub_prop_get(Object *obj, Visitor *v, const char *name, void *opaque, Error **errp)
{
    switch_log("STUB: stub_prop_get called for property '%s'\n", name);
    (void)obj; (void)v; (void)name; (void)opaque; (void)errp;
}

static void stub_prop_set(Object *obj, Visitor *v, const char *name, void *opaque, Error **errp)
{
    switch_log("STUB: stub_prop_set called for property '%s'\n", name);
    (void)obj; (void)v; (void)name; (void)opaque; (void)errp;
}

/* Stub create function that properly creates a property using object_class_property_add */
static ObjectProperty *stub_prop_create(ObjectClass *oc, const char *name, const Property *prop)
{
    switch_log("STUB: stub_prop_create called for property '%s'\n", name);
    return object_class_property_add(oc, name, prop->info->name,
                                      stub_prop_get,
                                      stub_prop_set,
                                      NULL, /* release */
                                      (void *)prop); /* opaque */
}

/* Stub set_default_value function */
static void stub_prop_set_default_value(ObjectProperty *op, const Property *prop)
{
    switch_log("STUB: stub_prop_set_default_value called for property '%s'\n", prop->name);
    (void)op; (void)prop;
    /* Do nothing - stub implementation */
}

/* PropertyInfo stubs for network/char devices - provide minimal implementation */
__attribute__((weak)) const PropertyInfo qdev_prop_chr = {
    .name = "chr",
    .description = "Character device (stub)",
    .get = stub_prop_get,
    .set = stub_prop_set,
    .set_default_value = stub_prop_set_default_value,
    .create = stub_prop_create,
};
__attribute__((weak)) const PropertyInfo qdev_prop_macaddr = {
    .name = "macaddr",
    .description = "MAC address (stub)",
    .get = stub_prop_get,
    .set = stub_prop_set,
    .set_default_value = stub_prop_set_default_value,
    .create = stub_prop_create,
};
__attribute__((weak)) const PropertyInfo qdev_prop_netdev = {
    .name = "netdev",
    .description = "Network device (stub)",
    .get = stub_prop_get,
    .set = stub_prop_set,
    .set_default_value = stub_prop_set_default_value,
    .create = stub_prop_create,
};

__attribute__((weak)) void qdev_connect_gpio_out(DeviceState *dev, int n, qemu_irq pin)
{
    switch_log("STUB: qdev_connect_gpio_out called\n");
    (void)dev; (void)n; (void)pin;
}

__attribute__((weak)) void qdev_connect_gpio_out_named(DeviceState *dev,
                                                       const char *name,
                                                       int n,
                                                       qemu_irq pin)
{
    switch_log("STUB: qdev_connect_gpio_out_named called\n");
    (void)dev;
    (void)name;
    (void)n;
    (void)pin;
}

void qdev_finalize_clocklist(DeviceState *dev)
{
    switch_log("STUB: qdev_finalize_clocklist called\n");
    (void)dev;
}

const char *qdev_fw_name(DeviceState *dev)
{
    switch_log("STUB: qdev_fw_name called\n");
    (void)dev;
    return NULL;
}

/*
 * QemuOpts definitions for system/vl.c and other QEMU components.
 * These are normally provided by various source files (qdev-monitor.c,
 * net/net.c, monitor/monitor.c, hw/core/numa.c), but those files have
 * complex dependencies that aren't available in the Switch build.
 *
 * NOTE: These must be actual struct definitions with .name fields,
 * NOT NULL pointers, because find_list() in qemu-config.c iterates
 * through vm_config_groups[] and accesses ->name on each entry.
 *
 * These stubs are still needed because the source files that normally
 * provide them (qdev-monitor.c, etc.) are not included in the build.
 */

#include "qemu/queue.h"

QemuOptsList qemu_device_opts = {
    .name = "device",
    .implied_opt_name = "driver",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_device_opts.head),
    .desc = {
        { /* end of list */ }
    },
};

QemuOptsList qemu_global_opts = {
    .name = "global",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_global_opts.head),
    .desc = {
        { /* end of list */ }
    },
};

QemuOptsList qemu_netdev_opts = {
    .name = "netdev",
    .implied_opt_name = "type",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_netdev_opts.head),
    .desc = {
        { /* end of list */ }
    },
};

QemuOptsList qemu_nic_opts = {
    .name = "nic",
    .implied_opt_name = "type",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_nic_opts.head),
    .desc = {
        { /* end of list */ }
    },
};

QemuOptsList qemu_net_opts = {
    .name = "net",
    .implied_opt_name = "type",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_net_opts.head),
    .desc = {
        { /* end of list */ }
    },
};

QemuOptsList qemu_mon_opts = {
    .name = "mon",
    .implied_opt_name = "chardev",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_mon_opts.head),
    .desc = {
        { /* end of list */ }
    },
};

QemuOptsList qemu_numa_opts = {
    .name = "numa",
    .implied_opt_name = "type",
    .head = QTAILQ_HEAD_INITIALIZER(qemu_numa_opts.head),
    .desc = {
        { /* end of list */ }
    },
};

bool qemu_global_option(const char *name)
{
    switch_log("STUB: qemu_global_option called\n");
    (void)name;
    return false;
}
