/*
 * OpenGL Loader Abstraction
 *
 * Provides a unified interface for OpenGL loading across platforms.
 * On Switch: Uses glad (via switch-glad package)
 * On Desktop: Uses libepoxy
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef GL_LOADER_H
#define GL_LOADER_H

#ifdef CONFIG_SWITCH
/*
 * On Nintendo Switch, use glad for OpenGL loading.
 * glad is provided by the switch-glad devkitpro package.
 */
#include <glad/glad.h>

/* Compatibility macros for epoxy-style version checks */
#define GL_LOADER_VERSION_MAJOR GLVersion.major
#define GL_LOADER_VERSION_MINOR GLVersion.minor
#define GL_VERSION_CHECK(major, minor) \
    ((GLVersion.major > (major)) || \
     (GLVersion.major == (major) && GLVersion.minor >= (minor)))
#define GL_VERSION_40_OR_HIGHER GL_VERSION_CHECK(4, 0)

#else
/*
 * On desktop platforms, use libepoxy for OpenGL loading.
 */
#include <epoxy/gl.h>

/* Compatibility macros using epoxy's version check */
#define GL_LOADER_VERSION_MAJOR (epoxy_gl_version() / 10)
#define GL_LOADER_VERSION_MINOR (epoxy_gl_version() % 10)
#define GL_VERSION_CHECK(major, minor) \
    (epoxy_gl_version() >= ((major) * 10 + (minor)))
#define GL_VERSION_40_OR_HIGHER (epoxy_gl_version() >= 40)

#endif /* CONFIG_SWITCH */

#endif /* GL_LOADER_H */
