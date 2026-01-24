/*
 * QEMU Stubs for Nintendo Switch - OS/POSIX Compatibility
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Error reporting stubs
 * Note: error_printf is already provided by util/error-report.c
 */

static pthread_t switch_main_thread;

void switch_set_main_thread(pthread_t t)
{
    switch_main_thread = t;
}

void error_vprintf(const char *fmt, va_list ap)
{
    char buf[1024];
    va_list ap_copy;

    va_copy(ap_copy, ap);
    vsnprintf(buf, sizeof(buf), fmt, ap_copy);
    va_end(ap_copy);

    switch_log("%s", buf);
}

__attribute__((noreturn)) void exit(int status)
{
    switch_log("exit(%d) called\n", status);
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
}

void os_set_proc_name(const char *s)
{
    (void)s;
}

int os_set_daemonize(bool d)
{
    (void)d;
    return 0;
}

bool os_set_runas(const char *user_id)
{
    (void)user_id;
    return true;
}

void os_set_chroot(const char *path)
{
    (void)path;
}

bool qemu_finish_async_prealloc_mem(Error **errp)
{
    (void)errp;
    return true;
}

char *qemu_get_pid_name(pid_t pid)
{
    (void)pid;
    return NULL;
}

int socket_init(void)
{
    return 0;
}
