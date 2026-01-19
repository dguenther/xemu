/*
 * sys/uio.h stub for Nintendo Switch (devkitpro/libnx)
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef _SWITCH_SYS_UIO_H
#define _SWITCH_SYS_UIO_H

#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/* libnx already defines struct iovec in sys/_iovec.h, include it directly */
#ifdef __SWITCH__
#include <sys/_iovec.h>
#else
struct iovec {
    void  *iov_base;  /* Starting address */
    size_t iov_len;   /* Number of bytes */
};
#endif

static inline ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t total = 0;
    for (int i = 0; i < iovcnt; i++) {
        ssize_t r = read(fd, iov[i].iov_base, iov[i].iov_len);
        if (r < 0) return r;
        total += r;
        if ((size_t)r < iov[i].iov_len) break;
    }
    return total;
}

static inline ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t total = 0;
    for (int i = 0; i < iovcnt; i++) {
        ssize_t w = write(fd, iov[i].iov_base, iov[i].iov_len);
        if (w < 0) return w;
        total += w;
        if ((size_t)w < iov[i].iov_len) break;
    }
    return total;
}

static inline ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    (void)fd; (void)iov; (void)iovcnt; (void)offset;
    errno = ENOSYS;
    return -1;
}

static inline ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    (void)fd; (void)iov; (void)iovcnt; (void)offset;
    errno = ENOSYS;
    return -1;
}

#ifdef __cplusplus
}
#endif

#endif /* _SWITCH_SYS_UIO_H */
