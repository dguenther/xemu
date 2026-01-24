/*
 * QEMU Stubs for Nintendo Switch - Xbox SMC Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"

/* Xbox SMC functions */
void __attribute__((weak)) xbox_smc_eject_button(void)
{
}

void __attribute__((weak)) xbox_smc_update_tray_state(void)
{
}