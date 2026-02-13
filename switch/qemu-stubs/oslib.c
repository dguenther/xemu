/*
 * QEMU Stubs for Nintendo Switch - OS/POSIX Compatibility
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef uint32_t NxResult;

typedef struct {
    uint64_t addr;
    uint64_t size;
    uint32_t type;
    uint32_t attr;
    uint32_t perm;
    uint32_t ipc_refcount;
    uint32_t device_refcount;
    uint32_t padding;
} NxMemoryInfo;

extern NxResult svcQueryMemory(NxMemoryInfo *mem_info, uint32_t *page_info,
                               uint64_t addr);

/*
 * Error reporting stubs
 * Note: error_printf is already provided by util/error-report.c
 */

static pthread_t switch_main_thread;

void switch_set_main_thread(pthread_t t)
{
    switch_main_thread = t;
}

bool switch_is_main_thread(void)
{
    return switch_main_thread &&
           pthread_equal(pthread_self(), switch_main_thread);
}

__attribute__((noreturn)) void exit(int status)
{
    void *ret = __builtin_return_address(0);
    NxMemoryInfo mem_info;
    uint32_t page_info = 0;
    NxResult rc = svcQueryMemory(&mem_info, &page_info, (uint64_t)ret);
    uintptr_t base = (rc == 0) ? (uintptr_t)mem_info.addr : 0;
    uintptr_t off = base ? ((uintptr_t)ret - base) : 0;
    if (rc == 0) {
        switch_log("exit(%d) called (return=%p base=%p off=0x%lx)\n",
                   status, ret, (void *)base, (unsigned long)off);
    } else {
        switch_log("exit(%d) called (return=%p query failed: 0x%x)\n",
                   status, ret, rc);
    }
    if (switch_main_thread &&
        !pthread_equal(pthread_self(), switch_main_thread)) {
        pthread_exit((void *)(intptr_t)status);
    }
    _Exit(status);
}

__attribute__((noreturn)) void abort(void)
{
    switch_log("abort() called\n");
    if (switch_main_thread &&
        !pthread_equal(pthread_self(), switch_main_thread)) {
        pthread_exit((void *)(intptr_t)1);
    }
    _Exit(1);
}

/*
 * pthread_sigmask stub
 * Signals are not fully supported on Switch
 */
int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset)
{
    (void)how;
    (void)set;
    (void)oldset;
    return 0; /* Success */
}

/*
 * posix_memalign stub
 */
int posix_memalign(void **memptr, size_t alignment, size_t size)
{
    if (!memptr) {
        return EINVAL;
    }
    if (alignment < sizeof(void *) || (alignment & (alignment - 1)) != 0) {
        return EINVAL;
    }
    void *ptr = malloc(size);
    if (!ptr) {
        return ENOMEM;
    }
    *memptr = ptr;
    return 0;
}

/*
 * File locking stubs
 * These are POSIX functions not available in newlib
 */
void flockfile(FILE *file)
{
    (void)file;
    /* No-op on Switch - single-threaded file access assumed */
}

void funlockfile(FILE *file)
{
    (void)file;
}

int ftrylockfile(FILE *file)
{
    (void)file;
    return 0; /* Success */
}

/*
 * sysconf stub
 * Returns system configuration values
 */
long sysconf(int name)
{
    /* Common sysconf values */
    switch (name) {
    case 30: /* _SC_PAGESIZE / _SC_PAGE_SIZE */
        return 4096;
    case 84: /* _SC_LEVEL1_DCACHE_SIZE */
        return 32768; /* 32KB L1 data cache for Cortex-A57 */
    case 85: /* _SC_LEVEL1_DCACHE_ASSOC */
        return 2;
    case 86: /* _SC_LEVEL1_DCACHE_LINESIZE */
        return 64;
    case 87: /* _SC_LEVEL1_ICACHE_SIZE */
        return 49152; /* 48KB L1 instruction cache for Cortex-A57 */
    case 88: /* _SC_LEVEL1_ICACHE_ASSOC */
        return 3;
    case 89: /* _SC_LEVEL1_ICACHE_LINESIZE */
        return 64;
    case 90: /* _SC_LEVEL2_CACHE_SIZE */
        return 2097152; /* 2MB L2 cache */
    case 91: /* _SC_LEVEL2_CACHE_ASSOC */
        return 16;
    case 92: /* _SC_LEVEL2_CACHE_LINESIZE */
        return 64;
    default:
        return -1;
    }
}

/*
 * Memory allocation stub
 */
void *qemu_memalign(size_t alignment, size_t size)
{
    void *ptr = NULL;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return NULL;
    }
    return ptr;
}

void os_setup_limits(void)
{
    switch_log("STUB: os_setup_limits called\n");
}

void os_set_proc_name(const char *s)
{
    switch_log("STUB: os_set_proc_name called\n");
    (void)s;
}

int os_set_daemonize(bool d)
{
    switch_log("STUB: os_set_daemonize called\n");
    (void)d;
    return 0;
}

bool os_set_runas(const char *user_id)
{
    switch_log("STUB: os_set_runas called\n");
    (void)user_id;
    return true;
}

void os_set_chroot(const char *path)
{
    switch_log("STUB: os_set_chroot called\n");
    (void)path;
}

bool qemu_finish_async_prealloc_mem(Error **errp)
{
    switch_log("STUB: qemu_finish_async_prealloc_mem called\n");
    (void)errp;
    return true;
}

char *qemu_get_pid_name(pid_t pid)
{
    switch_log("STUB: qemu_get_pid_name called\n");
    (void)pid;
    return NULL;
}

int socket_init(void)
{
    switch_log("STUB: socket_init called\n");
    return 0;
}
