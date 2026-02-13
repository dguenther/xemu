/*
 * QEMU Stubs for Nintendo Switch - VMState and Migration
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * VMState info stubs
 */
__attribute__((weak)) const VMStateInfo vmstate_info_uint16 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_uint8 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_int64 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_timer = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_bool = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_int16 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_int32 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_uint32 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_uint64 = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_unused_buffer = { 0 };
__attribute__((weak)) const VMStateInfo vmstate_info_uint8_equal = { 0 };

static int vmstate_stub_get(QEMUFile *f, void *pv, size_t size,
                            const VMStateField *field)
{
    switch_log("STUB: vmstate_stub_get called\n");
    (void)f;
    (void)pv;
    (void)size;
    (void)field;
    return 0;
}

static int vmstate_stub_put(QEMUFile *f, void *pv, size_t size,
                            const VMStateField *field, JSONWriter *vmdesc)
{
    switch_log("STUB: vmstate_stub_put called\n");
    (void)f;
    (void)pv;
    (void)size;
    (void)field;
    (void)vmdesc;
    return 0;
}
