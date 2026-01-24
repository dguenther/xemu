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
    (void)backend;
    return NULL;
}

bool host_memory_backend_is_mapped(HostMemoryBackend *backend)
{
    (void)backend;
    return false;
}

void host_memory_backend_set_mapped(HostMemoryBackend *backend, bool mapped)
{
    (void)backend;
    (void)mapped;
}
