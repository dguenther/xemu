/*
 * Nintendo Switch OS-specific functions for xemu
 *
 * This file provides Switch/libnx implementations of platform-specific
 * functions required by QEMU/xemu.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "config-switch.h"
#include "platform_stubs.h"  /* Must come before glib-compat for mmap stubs */
#include "glib-compat.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#ifdef __SWITCH__
#include <switch.h>
#endif

typedef struct Error Error;

/*
 * Thread ID handling
 * libnx uses pthreads, so we use pthread_self()
 */
int qemu_get_thread_id(void)
{
    return (int)(uintptr_t)pthread_self();
}

/*
 * Daemon mode - not supported on Switch, just succeed silently
 */
int qemu_daemon(int nochdir, int noclose)
{
    (void)nochdir;
    (void)noclose;
    return 0;
}

/*
 * PID file - not needed on Switch
 */
bool qemu_write_pidfile(const char *path, Error **errp)
{
    (void)path;
    (void)errp;
    return true;
}

/*
 * Memory allocation
 * libnx provides mmap for memory mapping
 */

/* Page alignment for the platform */
#define SWITCH_PAGE_SIZE 4096

void *qemu_anon_ram_alloc(size_t size, uint64_t *alignment, bool shared,
                          bool noreserve)
{
    (void)shared;
    (void)noreserve;

    size_t align = SWITCH_PAGE_SIZE;

    /* Round up size to page boundary */
    size = (size + align - 1) & ~(align - 1);

    void *ptr = aligned_alloc(align, size);
    if (ptr == NULL) {
        return NULL;
    }

    memset(ptr, 0, size);

    if (alignment) {
        *alignment = align;
    }

    return ptr;
}

void qemu_anon_ram_free(void *ptr, size_t size)
{
    (void)size;
    free(ptr);
}

/*
 * Virtual memory allocation
 */
void *qemu_vmalloc(size_t size)
{
    return qemu_anon_ram_alloc(size, NULL, false, false);
}

void qemu_vfree(void *ptr)
{
    free(ptr);
}

void *qemu_try_memalign(size_t alignment, size_t size)
{
    void *ptr = NULL;

    if (alignment < sizeof(void *)) {
        alignment = sizeof(void *);
    }
    if (size == 0) {
        size = 1;
    }

    if (posix_memalign(&ptr, alignment, size) != 0) {
        return NULL;
    }
    return ptr;
}

/*
 * Memory region helper - for huge pages etc.
 * Switch doesn't support huge pages, so this just falls back to regular alloc
 */
void *qemu_ram_mmap(int fd, size_t size, size_t align, uint32_t flags, off_t offset)
{
    (void)fd;
    (void)flags;
    (void)offset;

    if (align < SWITCH_PAGE_SIZE) {
        align = SWITCH_PAGE_SIZE;
    }

    size = (size + align - 1) & ~(align - 1);
    void *ptr = aligned_alloc(align, size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

void qemu_ram_munmap(int fd, void *ptr, size_t size)
{
    (void)fd;
    (void)size;
    free(ptr);
}

/*
 * Memory protection
 */
int qemu_mprotect_rw(void *addr, size_t size)
{
    (void)addr;
    (void)size;
    return 0; /* Success - no-op on Switch for now */
}

int qemu_mprotect_rwx(void *addr, size_t size)
{
    (void)addr;
    (void)size;
    return 0;
}

int qemu_mprotect_none(void *addr, size_t size)
{
    (void)addr;
    (void)size;
    return 0;
}

/*
 * Memory advice - not available on Switch
 */
int qemu_madvise(void *addr, size_t len, int advice)
{
    (void)addr;
    (void)len;
    (void)advice;
    return 0;
}

/*
 * File descriptor handling
 */
void qemu_set_cloexec(int fd)
{
    int flags = fcntl(fd, F_GETFD);
    if (flags >= 0) {
        fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
    }
}

int qemu_set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        return -errno;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -errno;
    }
    return 0;
}

int qemu_set_block(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        return -errno;
    }
    if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) < 0) {
        return -errno;
    }
    return 0;
}

/*
 * Socket utilities
 */
int socket_set_cork(int fd, int v)
{
    (void)fd;
    (void)v;
    return 0; /* TCP cork not available */
}

int socket_set_nodelay(int fd)
{
    (void)fd;
    return 0; /* Not available/needed */
}

/*
 * File creation with permissions
 */
int qemu_create(const char *path, int flags, mode_t mode, Error **errp)
{
    int fd = open(path, flags | O_CREAT, mode);
    if (fd < 0 && errp) {
        /* Error handling would go here */
    }
    return fd;
}

int qemu_open(const char *name, int flags, Error **errp)
{
    int fd = open(name, flags, 0666);
    if (fd < 0 && errp) {
        /* Error handling would go here */
    }
    return fd;
}

int qemu_open_old(const char *name, int flags, ...)
{
    int fd;
    if (flags & O_CREAT) {
        va_list ap;
        mode_t mode;
        va_start(ap, flags);
        mode = (mode_t)va_arg(ap, int);
        va_end(ap);
        fd = open(name, flags, mode);
    } else {
        fd = open(name, flags);
    }
    return fd;
}

int qemu_close(int fd)
{
    return close(fd);
}

int qemu_unlink(const char *name)
{
    return unlink(name);
}

int qemu_mkdir(const char *path)
{
    return mkdir(path, 0755);
}

int qemu_dup(int fd)
{
    return dup(fd);
}

int qemu_dup_flags(int fd, int flags)
{
    int newfd = dup(fd);
    if (newfd < 0) {
        return newfd;
    }
    if (flags & O_CLOEXEC) {
        qemu_set_cloexec(newfd);
    }
    return newfd;
}

int qemu_lock_fd(int fd, int64_t start, int64_t len, bool exclusive)
{
    (void)fd;
    (void)start;
    (void)len;
    (void)exclusive;
    return 0;
}

int qemu_unlock_fd(int fd, int64_t start, int64_t len)
{
    (void)fd;
    (void)start;
    (void)len;
    return 0;
}

int qemu_lock_fd_test(int fd, int64_t start, int64_t len, bool exclusive)
{
    (void)fd;
    (void)start;
    (void)len;
    (void)exclusive;
    return 0;
}

bool qemu_has_ofd_lock(void)
{
    return false;
}

int qemu_fdatasync(int fd)
{
    return fsync(fd);
}

__attribute__((weak)) ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    off_t cur = lseek(fd, 0, SEEK_CUR);
    if (cur == (off_t)-1) {
        return -1;
    }
    if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
        return -1;
    }
    ssize_t ret = read(fd, buf, count);
    lseek(fd, cur, SEEK_SET);
    return ret;
}

__attribute__((weak)) ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    off_t cur = lseek(fd, 0, SEEK_CUR);
    if (cur == (off_t)-1) {
        return -1;
    }
    if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
        return -1;
    }
    ssize_t ret = write(fd, buf, count);
    lseek(fd, cur, SEEK_SET);
    return ret;
}

/*
 * File writing helper
 */
ssize_t qemu_write_full(int fd, const void *buf, size_t count)
{
    const uint8_t *p = buf;
    size_t remaining = count;

    while (remaining > 0) {
        ssize_t ret = write(fd, p, remaining);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (ret == 0) {
            break; /* EOF */
        }
        p += ret;
        remaining -= ret;
    }

    return count - remaining;
}

/*
 * Time and sleep utilities
 */
void qemu_clk_wait(void)
{
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000 }; /* 1ms */
    nanosleep(&ts, NULL);
}

/*
 * Preallocation helpers - not needed on Switch
 */
int qemu_prealloc_mem(int fd, char *area, size_t sz, int max_threads,
                      int smp_cpus, bool async, void **errp)
{
    (void)fd;
    (void)area;
    (void)sz;
    (void)max_threads;
    (void)smp_cpus;
    (void)async;
    (void)errp;
    return 0;
}

/*
 * Secure memory functions
 */
void qemu_memset_s(void *s, size_t smax, int c, size_t n)
{
    if (n > smax) {
        n = smax;
    }
    volatile uint8_t *p = s;
    while (n--) {
        *p++ = c;
    }
}

/*
 * Thread CPU affinity - not available on Switch
 */
void qemu_set_thread_affinity(int cpu_index)
{
    (void)cpu_index;
}

/*
 * Get host physical page size
 */
size_t qemu_get_host_physmem(void)
{
    /* Return a reasonable amount for Switch - 3GB available to homebrew */
    return 3ULL * 1024 * 1024 * 1024;
}

long qemu_get_page_size(void)
{
    return SWITCH_PAGE_SIZE;
}

/*
 * Misc OS functions
 */
int qemu_init_main_loop(void **errp)
{
    (void)errp;
    return 0;
}

void os_setup_early_signal_handling(void)
{
    /* No signals on Switch */
}

void os_setup_signal_handling(void)
{
    /* No signals on Switch */
}

void os_daemonize(void)
{
    /* Cannot daemonize on Switch */
}

void os_setup_post(void)
{
    /* Post-fork setup - nothing needed on Switch */
}

int os_mlock(void)
{
    return 0; /* Memory is effectively locked on Switch */
}

void os_set_line_buffering(void)
{
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IOLBF, 0);
}

bool is_daemonized(void)
{
    return false;
}

/*
 * Temporary file creation
 */
int qemu_mkstemp(char *templ)
{
    /* Simple implementation for Switch */
    static int counter = 0;
    size_t len = strlen(templ);

    if (len < 6) {
        errno = EINVAL;
        return -1;
    }

    /* Replace XXXXXX with random-ish suffix */
    snprintf(templ + len - 6, 7, "%06d", counter++);

    return open(templ, O_RDWR | O_CREAT | O_EXCL, 0600);
}

/*
 * Path handling
 */
char *qemu_get_exec_dir(void)
{
    return g_strdup("sdmc:/switch/xemu");
}

/*
 * Memory info
 * Note: This function is also in cutils.c for some platforms.
 * We use weak linkage so the cutils.c version can override if present.
 */
__attribute__((weak))
void qemu_init_exec_dir(const char *argv0)
{
    (void)argv0;
}

/*
 * Poll-based fd monitoring for Switch
 * This is a simplified version since we don't have epoll
 */

typedef struct SwitchFdMon {
    int *fds;
    short *events;
    int count;
    int capacity;
} SwitchFdMon;

static SwitchFdMon g_fdmon = { NULL, NULL, 0, 0 };

int switch_fdmon_add(int fd, int events)
{
    if (g_fdmon.count >= g_fdmon.capacity) {
        int new_cap = g_fdmon.capacity ? g_fdmon.capacity * 2 : 16;
        g_fdmon.fds = g_realloc(g_fdmon.fds, new_cap * sizeof(int));
        g_fdmon.events = g_realloc(g_fdmon.events, new_cap * sizeof(short));
        g_fdmon.capacity = new_cap;
    }

    g_fdmon.fds[g_fdmon.count] = fd;
    g_fdmon.events[g_fdmon.count] = (short)events;
    g_fdmon.count++;

    return 0;
}

int switch_fdmon_remove(int fd)
{
    for (int i = 0; i < g_fdmon.count; i++) {
        if (g_fdmon.fds[i] == fd) {
            /* Swap with last element */
            g_fdmon.count--;
            if (i < g_fdmon.count) {
                g_fdmon.fds[i] = g_fdmon.fds[g_fdmon.count];
                g_fdmon.events[i] = g_fdmon.events[g_fdmon.count];
            }
            return 0;
        }
    }
    return -1;
}

int switch_fdmon_poll(int timeout_ms)
{
    if (g_fdmon.count == 0) {
        if (timeout_ms > 0) {
            struct timespec ts = {
                .tv_sec = timeout_ms / 1000,
                .tv_nsec = (timeout_ms % 1000) * 1000000
            };
            nanosleep(&ts, NULL);
        }
        return 0;
    }

    struct pollfd *pfds = g_new(struct pollfd, g_fdmon.count);
    for (int i = 0; i < g_fdmon.count; i++) {
        pfds[i].fd = g_fdmon.fds[i];
        pfds[i].events = g_fdmon.events[i];
        pfds[i].revents = 0;
    }

    int ret = poll(pfds, g_fdmon.count, timeout_ms);

    g_free(pfds);
    return ret;
}

/*
 * Initialization/cleanup
 */
void switch_platform_init(void)
{
#ifdef __SWITCH__
    /* Initialize libnx services */
    /* Note: appletInit, socketInitialize etc. should be called by main-switch.c */
#endif
}

void switch_platform_cleanup(void)
{
#ifdef __SWITCH__
    /* Cleanup libnx services */
#endif
}
