/*
 * QEMU Stubs for Nintendo Switch - Image/File Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Image size helper
 */
__attribute__((weak))
int64_t get_image_size(const char *filename)
{
    if (!filename || !filename[0]) {
        return -1;
    }
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return -1;
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }
    long size = ftell(fp);
    fclose(fp);
    if (size < 0) {
        return -1;
    }
    return (int64_t)size;
}

__attribute__((weak))
ssize_t load_image_size(const char *filename, void *addr, size_t size)
{
    switch_log("STUB: load_image_size called\n");
    (void)filename;
    (void)addr;
    (void)size;
    return -1;
}

__attribute__((weak))
ssize_t rom_add_vga(const char *file)
{
    switch_log("STUB: rom_add_vga called\n");
    (void)file;
    return -1;
}

__attribute__((weak))
ssize_t rom_add_option(const char *file, int32_t bootindex)
{
    switch_log("STUB: rom_add_option called\n");
    (void)file;
    (void)bootindex;
    return -1;
}

__attribute__((weak))
void rom_set_order_override(int order)
{
    switch_log("STUB: rom_set_order_override called\n");
    (void)order;
}

__attribute__((weak))
void rom_reset_order_override(void)
{
    switch_log("STUB: rom_reset_order_override called\n");
}

__attribute__((weak))
int rom_check_and_register_reset(void)
{
    switch_log("STUB: rom_check_and_register_reset called\n");
    return 0;
}
