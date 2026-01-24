/*
 * QEMU Stubs for Nintendo Switch - Main QEMU Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/* Main QEMU functions */
void qemu_init(int argc, char **argv, char **envp)
{
    (void)argc; (void)argv; (void)envp;
}

/* qemu_main is a function pointer */
int (*qemu_main)(void) = NULL;