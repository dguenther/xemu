/*
 * QEMU Stubs for Nintendo Switch - Spice
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

int using_spice = 0;

static void qemu_spice_init_stub(void)
{
    switch_log("STUB: qemu_spice_init called\n");
}

static void qemu_spice_display_init_stub(void)
{
    switch_log("STUB: qemu_spice_display_init called\n");
}

static int qemu_spice_migrate_info_stub(const char *h, int p, int t,
                                        const char *s)
{
    switch_log("STUB: qemu_spice_migrate_info called\n");
    (void)h;
    (void)p;
    (void)t;
    (void)s;
    return -1;
}

static int qemu_spice_set_passwd_stub(const char *passwd,
                                      bool fail_if_connected,
                                      bool disconnect_if_connected)
{
    switch_log("STUB: qemu_spice_set_passwd called\n");
    (void)passwd;
    (void)fail_if_connected;
    (void)disconnect_if_connected;
    return -1;
}

static int qemu_spice_set_pw_expire_stub(time_t expires)
{
    switch_log("STUB: qemu_spice_set_pw_expire called\n");
    (void)expires;
    return -1;
}

static int qemu_spice_display_add_client_stub(int csock, int skipauth, int tls)
{
    switch_log("STUB: qemu_spice_display_add_client called\n");
    (void)csock;
    (void)skipauth;
    (void)tls;
    return -1;
}

struct QemuSpiceOps qemu_spice = {
    .init = qemu_spice_init_stub,
    .display_init = qemu_spice_display_init_stub,
    .migrate_info = qemu_spice_migrate_info_stub,
    .set_passwd = qemu_spice_set_passwd_stub,
    .set_pw_expire = qemu_spice_set_pw_expire_stub,
    .display_add_client = qemu_spice_display_add_client_stub,
};
