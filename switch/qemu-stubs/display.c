/*
 * QEMU Stubs for Nintendo Switch - Display/Console
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Display/console stubs
 */
typedef struct QemuDisplay QemuDisplay;
typedef struct DisplayChangeListener DisplayChangeListener;
typedef struct QemuConsole QemuConsole;

void qemu_display_register(QemuDisplay *ui)
{
    (void)ui;
}

void register_displaychangelistener(DisplayChangeListener *dcl)
{
    (void)dcl;
}

int qemu_console_get_index(QemuConsole *con)
{
    (void)con;
    return 0;
}

__attribute__((weak)) QemuConsole *qemu_console_lookup_by_index(unsigned int index)
{
    (void)index;
    return NULL;
}

__attribute__((weak)) bool qemu_console_is_graphic(QemuConsole *con)
{
    (void)con;
    return false;
}

bool qemu_display_find_default(DisplayOptions *opts)
{
    (void)opts;
    return false;
}

void qemu_display_early_init(DisplayOptions *opts)
{
    (void)opts;
}

const char *qemu_display_get_vc(DisplayOptions *opts)
{
    (void)opts;
    return NULL;
}

void qemu_display_help(void)
{
}

DisplayState *init_displaystate(void)
{
    return NULL;
}

void qemu_display_init(DisplayState *ds, DisplayOptions *opts)
{
    (void)ds;
    (void)opts;
}

/* Arch/init globals */
const uint32_t arch_type = QEMU_ARCH_I386;
int graphic_width = 800;
int graphic_height = 600;
int graphic_depth = 32;
