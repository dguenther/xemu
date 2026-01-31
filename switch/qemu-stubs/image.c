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

ssize_t load_image_size(const char *filename, void *addr, size_t size)
{
    (void)filename;
    (void)addr;
    (void)size;
    return -1;
}

ssize_t rom_add_vga(const char *file)
{
    (void)file;
    return -1;
}

ssize_t rom_add_option(const char *file, int32_t bootindex)
{
    (void)file;
    (void)bootindex;
    return -1;
}

void rom_set_order_override(const char *order)
{
    (void)order;
}

void rom_reset_order_override(void)
{
}

int rom_check_and_register_reset(void)
{
    return 0;
}
