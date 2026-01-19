/*
 * Nintendo Switch stub for pcap/pcap.h
 * Networking is not supported on Switch homebrew in the same way.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_PCAP_STUB_H
#define SWITCH_PCAP_STUB_H

#include <stddef.h>  /* For NULL */

/* Stub type for pcap interface list */
typedef struct pcap_if {
    struct pcap_if *next;
    char *name;
    char *description;
    void *addresses;
    unsigned int flags;
} pcap_if_t;

/* Stub pcap handle */
typedef void pcap_t;

/* Stub functions - all return failure or no-op */
static inline int pcap_findalldevs(pcap_if_t **alldevsp, char *errbuf)
{
    (void)errbuf;
    *alldevsp = NULL;
    return -1;  /* Return error - no devices available */
}

static inline void pcap_freealldevs(pcap_if_t *alldevs)
{
    (void)alldevs;
}

static inline pcap_t *pcap_open_live(const char *device, int snaplen,
                                      int promisc, int to_ms, char *errbuf)
{
    (void)device;
    (void)snaplen;
    (void)promisc;
    (void)to_ms;
    (void)errbuf;
    return NULL;
}

static inline void pcap_close(pcap_t *p)
{
    (void)p;
}

static inline char *pcap_geterr(pcap_t *p)
{
    (void)p;
    return (char *)"pcap not supported on Switch";
}

#endif /* SWITCH_PCAP_STUB_H */
