/*
 * QEMU Stubs for Nintendo Switch - USB
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef struct USBDevice USBDevice;

USBDevice *usbdevice_create(const char *cmdline)
{
    (void)cmdline;
    return NULL;
}
