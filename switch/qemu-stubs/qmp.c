/*
 * QEMU Stubs for Nintendo Switch - QMP/QAPI Commands
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

extern void vm_start(void);

/*
 * QMP command stubs (block device control)
 */
void qmp_cap_negotiation_commands()
{
    switch_log("STUB: qmp_cap_negotiation_commands called\n");
}
