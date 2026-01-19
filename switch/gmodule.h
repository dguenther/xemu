/*
 * gmodule.h stub for Nintendo Switch (devkitpro/libnx)
 * Dynamic module loading is not supported on Switch
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef _SWITCH_GMODULE_H
#define _SWITCH_GMODULE_H

#include "glib-compat.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GModule GModule;

typedef enum {
    G_MODULE_BIND_LAZY = 1 << 0,
    G_MODULE_BIND_LOCAL = 1 << 1,
    G_MODULE_BIND_MASK = 0x03
} GModuleFlags;

/* Module functions - all stubs since dynamic loading not supported */
static inline gboolean g_module_supported(void)
{
    return FALSE;
}

static inline GModule* g_module_open(const gchar *file_name, GModuleFlags flags)
{
    (void)file_name;
    (void)flags;
    return NULL;
}

static inline gboolean g_module_close(GModule *module)
{
    (void)module;
    return TRUE;
}

static inline gboolean g_module_symbol(GModule *module, const gchar *symbol_name, gpointer *symbol)
{
    (void)module;
    (void)symbol_name;
    (void)symbol;
    return FALSE;
}

static inline const gchar* g_module_error(void)
{
    return "Dynamic module loading not supported on Switch";
}

static inline const gchar* g_module_name(GModule *module)
{
    (void)module;
    return "";
}

static inline gchar* g_module_build_path(const gchar *directory, const gchar *module_name)
{
    (void)directory;
    return g_strdup(module_name);
}

#ifdef __cplusplus
}
#endif

#endif /* _SWITCH_GMODULE_H */
