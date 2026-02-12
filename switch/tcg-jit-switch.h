/*
 * Nintendo Switch JIT helpers for TCG split-WX support.
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_TCG_JIT_SWITCH_H
#define SWITCH_TCG_JIT_SWITCH_H

#include <stddef.h>
#include <stdint.h>

/*
 * Returns 0 on success; otherwise returns a libnx Result-style error code.
 */
uint32_t qemu_switch_tcg_jit_create(size_t size, void **buf_rw, void **buf_rx);
void qemu_switch_tcg_jit_write(void);
void qemu_switch_tcg_jit_execute(void);

#endif /* SWITCH_TCG_JIT_SWITCH_H */
