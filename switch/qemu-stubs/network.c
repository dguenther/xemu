/*
 * QEMU Stubs for Nintendo Switch - Network Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"
#include "net/net.h"

typedef struct SwitchNetClientState {
    NetClientState nc;
    NICState *nic;
    void *opaque;
} SwitchNetClientState;

void net_client_parse(QemuOptsList *opts_list, const char *optstr)
{
    switch_log("STUB: net_client_parse called\n");
    (void)opts_list;
    (void)optstr;
}

bool netdev_is_modern(const char *optstr)
{
    switch_log("STUB: netdev_is_modern called\n");
    (void)optstr;
    return false;
}

void netdev_parse_modern(const char *optstr)
{
    switch_log("STUB: netdev_parse_modern called\n");
    (void)optstr;
}

void net_init_clients(void)
{
    switch_log("STUB: net_init_clients called\n");
}

void net_cleanup(void)
{
    switch_log("STUB: net_cleanup called\n");
}

void net_check_clients(void)
{
    switch_log("STUB: net_check_clients called\n");
}

int qemu_find_net_clients_except(const char *id, NetClientState **ncs,
                                 NetClientDriver type, int max)
{
    switch_log("STUB: qemu_find_net_clients_except called\n");
    (void)id; (void)ncs; (void)type; (void)max;
    return 0;
}

NICState *qemu_new_nic(NetClientInfo *info,
                       NICConf *conf,
                       const char *model,
                       const char *name,
                       MemReentrancyGuard *reentrancy_guard,
                       void *opaque)
{
    switch_log("STUB: qemu_new_nic called\n");
    (void)model;
    (void)name;
    SwitchNetClientState *snc = calloc(1, sizeof(*snc));
    NICState *nic = calloc(1, sizeof(*nic));
    if (!snc || !nic) {
        free(snc);
        free(nic);
        return NULL;
    }

    snc->nc.info = info;
    snc->nc.link_down = 0;
    snc->opaque = opaque;
    snc->nic = nic;

    nic->ncs = &snc->nc;
    nic->conf = conf;
    nic->reentrancy_guard = reentrancy_guard;
    nic->opaque = opaque;

    return nic;
}

void qemu_del_nic(NICState *nic)
{
    switch_log("STUB: qemu_del_nic called\n");
    if (!nic) {
        return;
    }
    if (nic->ncs) {
        SwitchNetClientState *snc = container_of(nic->ncs, SwitchNetClientState, nc);
        free(snc);
    }
    free(nic);
}

NetClientState *qemu_get_queue(NICState *nic)
{
    switch_log("STUB: qemu_get_queue called\n");
    return nic ? nic->ncs : NULL;
}

NetClientState *qemu_get_subqueue(NICState *nic, int queue_index)
{
    switch_log("STUB: qemu_get_subqueue called\n");
    (void)queue_index;
    return qemu_get_queue(nic);
}

NICState *qemu_get_nic(NetClientState *nc)
{
    switch_log("STUB: qemu_get_nic called\n");
    if (!nc) {
        return NULL;
    }
    SwitchNetClientState *snc = container_of(nc, SwitchNetClientState, nc);
    return snc->nic;
}

void *qemu_get_nic_opaque(NetClientState *nc)
{
    switch_log("STUB: qemu_get_nic_opaque called\n");
    if (!nc) {
        return NULL;
    }
    SwitchNetClientState *snc = container_of(nc, SwitchNetClientState, nc);
    return snc->opaque;
}

void qemu_announce_self()
{
    switch_log("STUB: qemu_announce_self called\n");
}

void inet_parse()
{
    switch_log("STUB: inet_parse called\n");
}

void socket_parse()
{
    switch_log("STUB: socket_parse called\n");
}

void socket_start_incoming_migration()
{
    switch_log("STUB: socket_start_incoming_migration called\n");
}

void fd_is_socket()
{
    switch_log("STUB: fd_is_socket called\n");
}

void qio_channel_socket_new_fd()
{
    switch_log("STUB: qio_channel_socket_new_fd called\n");
}
