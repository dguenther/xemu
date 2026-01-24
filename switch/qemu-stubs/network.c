/*
 * QEMU Stubs for Nintendo Switch - Network Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/* Forward declarations */
typedef struct NetClientState NetClientState;
typedef enum NetClientDriver NetClientDriver;

void net_client_parse(QemuOptsList *opts_list, const char *optstr)
{
    (void)opts_list;
    (void)optstr;
}

bool netdev_is_modern(const char *optstr)
{
    (void)optstr;
    return false;
}

void netdev_parse_modern(const char *optstr)
{
    (void)optstr;
}

void net_init_clients(void)
{
}

void net_cleanup(void)
{
}

void net_check_clients(void)
{
}

int qemu_find_net_clients_except(const char *id, NetClientState **ncs, int type, int max)
{
    (void)id; (void)ncs; (void)type; (void)max;
    return 0;
}
