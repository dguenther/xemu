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

int vga_ioport_invalid(VGACommonState *s, uint32_t addr)
{
    if (s->msr & VGA_MIS_COLOR) {
        return (addr >= 0x3b0 && addr <= 0x3bf);
    } else {
        return (addr >= 0x3d0 && addr <= 0x3df);
    }
}

uint32_t vga_ioport_read(void *opaque, uint32_t addr)
{
    VGACommonState *s = opaque;
    int index;

    if (vga_ioport_invalid(s, addr)) {
        return 0xff;
    }

    switch (addr) {
    case VGA_ATT_W:
        return (s->ar_flip_flop == 0) ? s->ar_index : 0;
    case VGA_ATT_R:
        index = s->ar_index & 0x1f;
        return (index < VGA_ATT_C) ? s->ar[index] : 0;
    case VGA_MIS_W:
        return s->st00;
    case VGA_SEQ_I:
        return s->sr_index;
    case VGA_SEQ_D:
        return s->sr[s->sr_index];
    case VGA_PEL_IR:
        return s->dac_state;
    case VGA_PEL_IW:
        return s->dac_write_index;
    case VGA_PEL_D:
        index = s->palette[s->dac_read_index * 3 + s->dac_sub_index];
        if (++s->dac_sub_index == 3) {
            s->dac_sub_index = 0;
            s->dac_read_index++;
        }
        return index;
    case VGA_FTC_R:
        return s->fcr;
    case VGA_MIS_R:
        return s->msr;
    case VGA_GFX_I:
        return s->gr_index;
    case VGA_GFX_D:
        return s->gr[s->gr_index];
    case VGA_CRT_IM:
    case VGA_CRT_IC:
        return s->cr_index;
    case VGA_CRT_DM:
    case VGA_CRT_DC:
        return s->cr[s->cr_index];
    case VGA_IS1_RM:
    case VGA_IS1_RC:
        s->st01 ^= (ST01_V_RETRACE | ST01_DISP_ENABLE);
        s->ar_flip_flop = 0;
        return s->st01;
    default:
        return 0;
    }
}

void vga_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    VGACommonState *s = opaque;
    int index;

    if (vga_ioport_invalid(s, addr)) {
        return;
    }

    switch (addr) {
    case VGA_ATT_W:
        if (s->ar_flip_flop == 0) {
            s->ar_index = val & 0x3f;
        } else {
            index = s->ar_index & 0x1f;
            if (index < VGA_ATT_C) {
                s->ar[index] = val;
            }
        }
        s->ar_flip_flop ^= 1;
        break;
    case VGA_MIS_W:
        s->msr = val & ~0x10;
        break;
    case VGA_SEQ_I:
        s->sr_index = val & 7;
        break;
    case VGA_SEQ_D:
        s->sr[s->sr_index] = val;
        break;
    case VGA_PEL_IR:
        s->dac_read_index = val;
        s->dac_sub_index = 0;
        s->dac_state = 3;
        break;
    case VGA_PEL_IW:
        s->dac_write_index = val;
        s->dac_sub_index = 0;
        s->dac_state = 0;
        break;
    case VGA_PEL_D:
        s->dac_cache[s->dac_sub_index] = val;
        if (++s->dac_sub_index == 3) {
            memcpy(&s->palette[s->dac_write_index * 3], s->dac_cache, 3);
            s->dac_sub_index = 0;
            s->dac_write_index++;
        }
        break;
    case VGA_GFX_I:
        s->gr_index = val & 0x0f;
        break;
    case VGA_GFX_D:
        s->gr[s->gr_index] = val;
        break;
    case VGA_CRT_IM:
    case VGA_CRT_IC:
        s->cr_index = val;
        break;
    case VGA_CRT_DM:
    case VGA_CRT_DC:
        s->cr[s->cr_index] = val;
        break;
    case VGA_IS1_RM:
    case VGA_IS1_RC:
        s->fcr = val & 0x10;
        break;
    default:
        break;
    }
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
    memset(s->sr, 0, sizeof(s->sr));
    memset(s->gr, 0, sizeof(s->gr));
    memset(s->cr, 0, sizeof(s->cr));
    memset(s->ar, 0, sizeof(s->ar));
    s->sr_index = 0;
    s->gr_index = 0;
    s->cr_index = 0;
    s->ar_index = 0;
    s->ar_flip_flop = 0;
    s->msr = VGA_MIS_COLOR;
    s->fcr = 0;
    s->st00 = 0;
    s->st01 = ST01_DISP_ENABLE;
    s->dac_state = 0;
    s->dac_sub_index = 0;
    s->dac_read_index = 0;
    s->dac_write_index = 0;
}
