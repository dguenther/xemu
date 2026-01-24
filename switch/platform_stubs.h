/*
 * Nintendo Switch platform stubs for xemu
 *
 * This file provides stub definitions for features that are truly missing
 * in devkitpro's newlib. Unlike Linux, newlib already provides most POSIX
 * functions (signal, fork, etc.) so we only need to fill in a few gaps.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_PLATFORM_STUBS_H
#define SWITCH_PLATFORM_STUBS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/types.h>
#include <float.h>

/*
 * sigjmp_buf - on Switch we don't have signal masks, so just alias to jmp_buf
 */
#ifndef sigjmp_buf
typedef jmp_buf sigjmp_buf;
#define sigsetjmp(env, savemask) setjmp(env)
#define siglongjmp(env, val) longjmp(env, val)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* sys/mman.h is provided as switch/sys/mman.h */
/* sys/uio.h is provided as switch/sys/uio.h */

/*
 * Resource limits - newlib has basic rlimit but may be missing some defines
 */
#ifndef RLIMIT_MEMLOCK
#define RLIMIT_MEMLOCK 8
#endif
#ifndef RLIMIT_AS
#define RLIMIT_AS 9
#endif
#ifndef RLIM_INFINITY
#define RLIM_INFINITY (~0UL)
#endif

/*
 * Syslog - not available on Switch
 */
#ifndef LOG_EMERG
#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7
#define LOG_PID     0x01
#define LOG_DAEMON  (3 << 3)
#define LOG_USER    (1 << 3)

static inline void openlog(const char *ident, int option, int facility)
{
    (void)ident; (void)option; (void)facility;
}

static inline void syslog(int priority, const char *format, ...)
{
    (void)priority; (void)format;
}

static inline void closelog(void)
{
}
#endif /* LOG_EMERG */

/*
 * Wait macros - ensure they're defined
 */
#ifndef WIFEXITED
#define WIFEXITED(status)   (((status) & 0x7f) == 0)
#endif
#ifndef WEXITSTATUS
#define WEXITSTATUS(status) (((status) & 0xff00) >> 8)
#endif
#ifndef WIFSIGNALED
#define WIFSIGNALED(status) (((status) & 0x7f) != 0 && ((status) & 0x7f) != 0x7f)
#endif
#ifndef WTERMSIG
#define WTERMSIG(status)    ((status) & 0x7f)
#endif
#ifndef WIFSTOPPED
#define WIFSTOPPED(status)  (((status) & 0xff) == 0x7f)
#endif
#ifndef WSTOPSIG
#define WSTOPSIG(status)    (((status) & 0xff00) >> 8)
#endif
#ifndef WCOREDUMP
#define WCOREDUMP(status)   ((status) & 0x80)
#endif
#ifndef WNOHANG
#define WNOHANG 1
#endif
#ifndef WUNTRACED
#define WUNTRACED 2
#endif

/*
 * Missing fcntl defines
 */
#ifndef F_GETLK
#define F_GETLK  5
#define F_SETLK  6
#define F_SETLKW 7
#endif
#ifndef F_RDLCK
#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2
#endif

/*
 * FD_CLOEXEC if not defined
 */
#ifndef FD_CLOEXEC
#define FD_CLOEXEC 1
#endif

/*
 * POSIX AIO (not available on Switch)
 */
#ifndef AIO_CANCELED
#define AIO_CANCELED    0
#define AIO_NOTCANCELED 1
#define AIO_ALLDONE     2
#endif

/*
 * Poll events - libnx provides poll.h so we don't define these
 */

/*
 * Additional signal numbers that may be missing
 */
#ifndef SIGWINCH
#define SIGWINCH 28
#endif
#ifndef SIGIO
#define SIGIO 29
#endif
#ifndef SIGPWR
#define SIGPWR 30
#endif
#ifndef SIGSYS
#define SIGSYS 31
#endif

/*
 * prctl - Linux-specific, stub it
 */
#ifndef PR_SET_NAME
#define PR_SET_NAME 15
#define PR_GET_NAME 16

static inline int prctl(int option, ...)
{
    (void)option;
    return 0;
}
#endif

/*
 * eventfd - Linux-specific, stub it
 */
#ifndef EFD_NONBLOCK
#define EFD_NONBLOCK 04000
#define EFD_CLOEXEC  02000000
#define EFD_SEMAPHORE 1

static inline int eventfd(unsigned int initval, int flags)
{
    (void)initval; (void)flags;
    errno = ENOSYS;
    return -1;
}
#endif

/*
 * signalfd - Linux-specific, stub it
 */
#ifndef SFD_NONBLOCK
#define SFD_NONBLOCK 04000
#define SFD_CLOEXEC  02000000

static inline int signalfd(int fd, const void *mask, int flags)
{
    (void)fd; (void)mask; (void)flags;
    errno = ENOSYS;
    return -1;
}
#endif

/*
 * timerfd - Linux-specific, stub it
 */
#ifndef TFD_NONBLOCK
#define TFD_NONBLOCK 04000
#define TFD_CLOEXEC  02000000
#define TFD_TIMER_ABSTIME 1

#define CLOCK_MONOTONIC 1
#define CLOCK_REALTIME  0

static inline int timerfd_create(int clockid, int flags)
{
    (void)clockid; (void)flags;
    errno = ENOSYS;
    return -1;
}
#endif

/*
 * memfd_create - Linux-specific
 */
#ifndef MFD_CLOEXEC
#define MFD_CLOEXEC 0x0001U

static inline int memfd_create(const char *name, unsigned int flags)
{
    (void)name; (void)flags;
    errno = ENOSYS;
    return -1;
}
#endif

/*
 * getrandom - not available, use stdlib random
 */
#ifndef GRND_NONBLOCK
#define GRND_NONBLOCK 0x0001
#define GRND_RANDOM   0x0002

static inline ssize_t getrandom(void *buf, size_t buflen, unsigned int flags)
{
    (void)flags;
    unsigned char *p = (unsigned char *)buf;
    for (size_t i = 0; i < buflen; i++) {
        p[i] = (unsigned char)rand();
    }
    return (ssize_t)buflen;
}
#endif

/*
 * pthread_setname_np - newlib provides this, so we don't need to stub it
 */

/*
 * Copy file range - Linux-specific
 */
#ifndef SWITCH_QEMU_CORE
static inline ssize_t copy_file_range(int fd_in, off_t *off_in,
                                      int fd_out, off_t *off_out,
                                      size_t len, unsigned int flags)
{
    (void)fd_in; (void)off_in; (void)fd_out; (void)off_out;
    (void)len; (void)flags;
    errno = ENOSYS;
    return -1;
}
#endif

/*
 * ioctl - not available on Switch
 */
#ifdef __SWITCH__
static inline int ioctl(int fd, unsigned long request, ...)
{
    (void)fd;
    (void)request;
    errno = ENOSYS;
    return -1;
}
#endif

/*
 * sendfile - Linux/BSD specific
 */
static inline ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count)
{
    (void)out_fd; (void)in_fd; (void)offset; (void)count;
    errno = ENOSYS;
    return -1;
}

/*
 * splice - Linux-specific
 */
#ifndef SPLICE_F_MOVE
#define SPLICE_F_MOVE     1
#define SPLICE_F_NONBLOCK 2
#define SPLICE_F_MORE     4
#define SPLICE_F_GIFT     8

static inline ssize_t splice(int fd_in, off_t *off_in, int fd_out,
                             off_t *off_out, size_t len, unsigned int flags)
{
    (void)fd_in; (void)off_in; (void)fd_out; (void)off_out;
    (void)len; (void)flags;
    errno = ENOSYS;
    return -1;
}
#endif

/*
 * timegm - GNU extension for converting tm to time_t in UTC
 */
#include <time.h>
#ifndef timegm
static inline time_t timegm(struct tm *tm)
{
    /* Simple implementation: use mktime and adjust for timezone */
    time_t t = mktime(tm);
    /* This is a simplification - doesn't handle all edge cases */
    return t;
}
#endif

/*
 * getpagesize - POSIX function
 * Declared but not implemented in newlib, provide stub
 */
#ifndef getpagesize
#define getpagesize() 4096
#endif

/*
 * pthread_kill - send signal to thread (not supported on Switch)
 * Declared but may not be fully implemented in newlib
 */
#ifndef __SWITCH_PTHREAD_KILL_STUB
#define __SWITCH_PTHREAD_KILL_STUB
#include <signal.h>
#include <pthread.h>
#ifdef __SWITCH__
#define pthread_kill(thread, sig) 0
#endif
#endif

#ifdef __cplusplus
}
#endif

/*
 * QEMU macros needed for C++ compilation when including QEMU headers
 * These are normally defined in qemu/compiler.h but that file has C-specific constructs
 */
#ifndef unlikely
#define unlikely(x)   __builtin_expect(!!(x), 0)
#endif

#ifndef likely
#define likely(x)     __builtin_expect(!!(x), 1)
#endif

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#endif

/*
 * coroutine_fn - marks functions that can be called from coroutines
 * On Switch, we don't use coroutines, so this is a no-op
 */
#ifndef coroutine_fn
#define coroutine_fn
#endif

#endif /* SWITCH_PLATFORM_STUBS_H */
