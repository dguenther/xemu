/*
 * QEMU Stubs for Nintendo Switch - Main QEMU Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * These stubs are only used when NOT building the QEMU core.
 * When SWITCH_QEMU_CORE is defined, the real QEMU provides these symbols.
 */
#ifndef SWITCH_QEMU_CORE

/* Main QEMU functions */
void qemu_init(int argc, char **argv, char **envp)
{
    (void)argc; (void)argv; (void)envp;
}

/* qemu_main is a function pointer */
static int qemu_default_main(void) { return 0; }
int (*qemu_main)(void) = qemu_default_main;

#endif /* !SWITCH_QEMU_CORE */