/*
 * QEMU Stubs for Nintendo Switch - POSIX Process Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * POSIX process stubs
 * waitpid and execvp are not available on Switch
 */

int waitpid(int pid, int *status, int options)
{
    switch_log("STUB: waitpid called\n");
    (void)pid;
    (void)status;
    (void)options;
    return -1;  /* Always fail - no process support */
}

int execvp(const char *file, char *const argv[])
{
    switch_log("STUB: execvp called\n");
    (void)file;
    (void)argv;
    return -1;  /* Always fail - no process support */
}
