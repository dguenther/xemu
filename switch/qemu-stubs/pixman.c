/*
 * QEMU Stubs for Nintendo Switch - Pixman helpers
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "ui/qemu-pixman.h"

void qemu_pixman_image_unref(pixman_image_t *image)
{
    if (image) {
        pixman_image_unref(image);
    }
}

bool qemu_pixman_image_new_shareable(pixman_image_t **image,
                                     qemu_pixman_shareable *handle,
                                     const char *name,
                                     pixman_format_code_t format,
                                     int width,
                                     int height,
                                     int rowstride_bytes,
                                     Error **errp)
{
    (void)name;
    if (handle) {
        *handle = SHAREABLE_NONE;
    }
    if (!image) {
        return false;
    }

    *image = pixman_image_create_bits(format, width, height, NULL,
                                      rowstride_bytes);
    if (!*image) {
        error_setg(errp, "pixman image allocation failed");
        return false;
    }

    return true;
}
