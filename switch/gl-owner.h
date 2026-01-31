/*
 * Switch GL ownership synchronization
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_GL_OWNER_H
#define SWITCH_GL_OWNER_H

void switch_gl_owner_lock(const char *tag);
void switch_gl_owner_unlock(const char *tag);
bool switch_gl_owner_trylock(const char *tag);

#endif /* SWITCH_GL_OWNER_H */
