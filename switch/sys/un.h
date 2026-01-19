/*
 * sys/un.h stub for Nintendo Switch (devkitpro/libnx)
 * Unix domain sockets - provide types but functions won't work
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef _SWITCH_SYS_UN_H
#define _SWITCH_SYS_UN_H

#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AF_UNIX
#define AF_UNIX 1
#endif

#ifndef AF_LOCAL
#define AF_LOCAL AF_UNIX
#endif

struct sockaddr_un {
    sa_family_t sun_family;  /* AF_UNIX */
    char sun_path[108];      /* Pathname */
};

#define SUN_LEN(ptr) ((size_t)(((struct sockaddr_un *)0)->sun_path) + strlen((ptr)->sun_path))

#ifdef __cplusplus
}
#endif

#endif /* _SWITCH_SYS_UN_H */
