/*
 * QEMU Stubs for Nintendo Switch - Memory Backend
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef struct MemoryRegion MemoryRegion;
typedef struct HostMemoryBackend HostMemoryBackend;

MemoryRegion *host_memory_backend_get_memory(HostMemoryBackend *backend)
{
    switch_log("STUB: host_memory_backend_get_memory called\n");
    (void)backend;
    return NULL;
}

bool host_memory_backend_is_mapped(HostMemoryBackend *backend)
{
    switch_log("STUB: host_memory_backend_is_mapped called\n");
    (void)backend;
    return false;
}

void host_memory_backend_set_mapped(HostMemoryBackend *backend, bool mapped)
{
    switch_log("STUB: host_memory_backend_set_mapped called\n");
    (void)backend;
    (void)mapped;
}
