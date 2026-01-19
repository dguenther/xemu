/*
 * sys/mman.h stub for Nintendo Switch (devkitpro/libnx)
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef _SWITCH_SYS_MMAN_H
#define _SWITCH_SYS_MMAN_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PROT_NONE   0x0
#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define PROT_EXEC   0x4

#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_FIXED     0x10
#define MAP_ANONYMOUS 0x20
#define MAP_ANON      MAP_ANONYMOUS
#define MAP_NORESERVE 0x40
#define MAP_FAILED    ((void *)-1)

#define MS_ASYNC      1
#define MS_SYNC       4
#define MS_INVALIDATE 2

#define MADV_NORMAL     0
#define MADV_RANDOM     1
#define MADV_SEQUENTIAL 2
#define MADV_WILLNEED   3
#define MADV_DONTNEED   4
#define MADV_FREE       8
#define MADV_HUGEPAGE   14
#define MADV_NOHUGEPAGE 15
#define MADV_DONTDUMP   16
#define MADV_DODUMP     17

#define MCL_CURRENT 1
#define MCL_FUTURE  2

static inline void *mmap(void *addr, size_t length, int prot, int flags,
                         int fd, off_t offset)
{
    (void)addr; (void)prot; (void)fd; (void)offset;
    /* No mmap on Switch - use malloc instead for anonymous mappings */
    if (flags & MAP_ANONYMOUS) {
        void *ptr = aligned_alloc(4096, length);
        if (ptr) {
            memset(ptr, 0, length);
        }
        return ptr ? ptr : MAP_FAILED;
    }
    errno = ENODEV;
    return MAP_FAILED;
}

static inline int munmap(void *addr, size_t length)
{
    (void)length;
    free(addr);
    return 0;
}

static inline int mprotect(void *addr, size_t len, int prot)
{
    (void)addr; (void)len; (void)prot;
    return 0; /* No-op - can't change protection on Switch */
}

static inline int madvise(void *addr, size_t length, int advice)
{
    (void)addr; (void)length; (void)advice;
    return 0;
}

static inline int posix_madvise(void *addr, size_t len, int advice)
{
    return madvise(addr, len, advice);
}

static inline int mlock(const void *addr, size_t len)
{
    (void)addr; (void)len;
    return 0;
}

static inline int munlock(const void *addr, size_t len)
{
    (void)addr; (void)len;
    return 0;
}

static inline int mlockall(int flags)
{
    (void)flags;
    return 0;
}

static inline int munlockall(void)
{
    return 0;
}

static inline int msync(void *addr, size_t length, int flags)
{
    (void)addr; (void)length; (void)flags;
    return 0;
}

static inline int mincore(void *addr, size_t length, unsigned char *vec)
{
    (void)addr; (void)length; (void)vec;
    errno = ENOSYS;
    return -1;
}

static inline void *mremap(void *old_addr, size_t old_size, size_t new_size, int flags, ...)
{
    (void)flags;
    void *new_addr = aligned_alloc(4096, new_size);
    if (new_addr) {
        size_t copy_size = old_size < new_size ? old_size : new_size;
        memcpy(new_addr, old_addr, copy_size);
        free(old_addr);
    }
    return new_addr ? new_addr : MAP_FAILED;
}

#ifdef __cplusplus
}
#endif

#endif /* _SWITCH_SYS_MMAN_H */
