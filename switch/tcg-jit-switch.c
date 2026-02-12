/*
 * Nintendo Switch JIT helpers for TCG split-WX support.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <switch/kernel/jit.h>

#include "tcg-jit-switch.h"

#define QEMU_SWITCH_JIT_ERR_ALIAS ((uint32_t)0xFFFFFFFFu)

static Jit tcg_switch_jit;
static bool tcg_switch_jit_valid;
typedef enum {
    SWITCH_JIT_MODE_UNKNOWN = 0,
    SWITCH_JIT_MODE_WRITE,
    SWITCH_JIT_MODE_EXEC,
} SwitchJitMode;
static __thread SwitchJitMode tcg_switch_jit_mode = SWITCH_JIT_MODE_UNKNOWN;

uint32_t qemu_switch_tcg_jit_create(size_t size, void **buf_rw, void **buf_rx)
{
    Result rc;

    if (!buf_rw || !buf_rx) {
        return QEMU_SWITCH_JIT_ERR_ALIAS;
    }

    rc = jitCreate(&tcg_switch_jit, size);
    if (rc != 0) {
        return (uint32_t)rc;
    }

    *buf_rw = jitGetRwAddr(&tcg_switch_jit);
    *buf_rx = jitGetRxAddr(&tcg_switch_jit);
    if (!*buf_rw || !*buf_rx) {
        (void)jitClose(&tcg_switch_jit);
        return QEMU_SWITCH_JIT_ERR_ALIAS;
    }

    rc = jitTransitionToWritable(&tcg_switch_jit);
    if (rc != 0) {
        (void)jitClose(&tcg_switch_jit);
        return (uint32_t)rc;
    }

    tcg_switch_jit_valid = true;
    tcg_switch_jit_mode = SWITCH_JIT_MODE_WRITE;
    return 0;
}

void qemu_switch_tcg_jit_write(void)
{
    Result rc;

    if (!tcg_switch_jit_valid) {
        return;
    }
    if (tcg_switch_jit_mode == SWITCH_JIT_MODE_WRITE) {
        return;
    }

    rc = jitTransitionToWritable(&tcg_switch_jit);
    if (rc == 0) {
        tcg_switch_jit_mode = SWITCH_JIT_MODE_WRITE;
    } else {
        static unsigned write_err_count;
        if (write_err_count < 10) {
            fprintf(stderr, "Switch: jitTransitionToWritable failed: 0x%x\n",
                    rc);
            write_err_count++;
        }
    }
}

void qemu_switch_tcg_jit_execute(void)
{
    Result rc;

    if (!tcg_switch_jit_valid) {
        return;
    }
    if (tcg_switch_jit_mode == SWITCH_JIT_MODE_EXEC) {
        return;
    }

    rc = jitTransitionToExecutable(&tcg_switch_jit);
    if (rc == 0) {
        tcg_switch_jit_mode = SWITCH_JIT_MODE_EXEC;
    } else {
        static unsigned exec_err_count;
        if (exec_err_count < 10) {
            fprintf(stderr, "Switch: jitTransitionToExecutable failed: 0x%x\n",
                    rc);
            exec_err_count++;
        }
    }
}
