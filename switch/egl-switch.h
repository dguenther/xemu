/*
 * Nintendo Switch EGL Context Helper
 *
 * Provides OpenGL 4.3 Core context creation using EGL on Switch.
 * Based on libnx examples.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef EGL_SWITCH_H
#define EGL_SWITCH_H

#include <stdbool.h>

#ifdef __SWITCH__
#include <switch.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initialize EGL with OpenGL 4.3 Core context.
 * On Switch, uses the default NWindow.
 * Returns true on success, false on failure.
 */
bool switch_egl_init(void);

/*
 * Swap the EGL buffers (present frame).
 */
void switch_egl_swap(void);

/*
 * Get the current EGL context (for passing to ImGui).
 * Returns NULL if not initialized.
 */
void *switch_egl_get_context(void);

/*
 * Get the display width.
 */
int switch_egl_get_width(void);

/*
 * Get the display height.
 */
int switch_egl_get_height(void);

/*
 * Clean up EGL resources.
 */
void switch_egl_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* EGL_SWITCH_H */
