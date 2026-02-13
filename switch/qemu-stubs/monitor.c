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

void monitor_init_hmp(const char *chardev_name, bool use_readline, Error **errp)
{
    switch_log("STUB: monitor_init_hmp called\n");
    (void)chardev_name;
    (void)use_readline;
    (void)errp;
}

typedef struct AddfdInfo AddfdInfo;
