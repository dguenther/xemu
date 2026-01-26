/*
 * Minimal VGA init for Nintendo Switch builds.
 *
 * Provides real signatures for VGA helpers that are stubbed as int in
 * switch/qemu-stubs/stub-int.c, and initializes enough state for NV2A.
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "exec/memory.h"
#include "hw/display/vga_int.h"
#include "hw/display/vga_regs.h"
#include "ui/console.h"

static void vga_switch_gfx_update(void *opaque)
{
    (void)opaque;
}

static const GraphicHwOps vga_switch_ops = {
    .gfx_update = vga_switch_gfx_update,
};

static void vga_switch_get_resolution(VGACommonState *s, int *pwidth, int *pheight)
{
    int width = (s->cr[VGA_CRTC_H_DISP] + 1) * 8;
    int height = s->cr[VGA_CRTC_V_DISP_END] |
        ((s->cr[VGA_CRTC_OVERFLOW] & 0x02) << 7) |
        ((s->cr[VGA_CRTC_OVERFLOW] & 0x40) << 3);
    height += 1;

    if (width < 64 || height < 64) {
        width = 640;
        height = 480;
    }

    *pwidth = width;
    *pheight = height;
}

bool vga_common_init(VGACommonState *s, Object *obj, Error **errp)
{
    Error *local_err = NULL;

    if (s->vram_size_mb == 0) {
        s->vram_size_mb = 1;
    }

    s->vram_size = s->vram_size_mb * MiB;
    memory_region_init_ram_nomigrate(&s->vram, obj, "vga.vram", s->vram_size,
                                     &local_err);
    if (local_err) {
        error_propagate(errp, local_err);
        return false;
    }

    s->vram_ptr = memory_region_get_ram_ptr(&s->vram);
    s->get_resolution = vga_switch_get_resolution;
    s->hw_ops = &vga_switch_ops;

    return true;
}

void vga_dirty_log_start(VGACommonState *s)
{
    (void)s;
}

void vga_common_reset(VGACommonState *s)
{
    (void)s;
}
