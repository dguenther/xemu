/*
 * QEMU Stubs for Nintendo Switch - Audio
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * libsamplerate stubs (audio resampling disabled)
 */
typedef struct SRC_STATE SRC_STATE;
typedef float (*src_callback_t)(void *cb_data, float **data);

__attribute__((weak)) SRC_STATE *src_callback_new(src_callback_t cb, int converter_type,
                                                   int channels, int *error,
                                                   void *cb_data)
{
    switch_log("STUB: src_callback_new called\n");
    (void)cb;
    (void)converter_type;
    (void)channels;
    (void)cb_data;
    if (error) {
        *error = 0;
    }
    return NULL;
}

__attribute__((weak)) long src_callback_read(SRC_STATE *state, double ratio,
                                              long frames, float *data)
{
    switch_log("STUB: src_callback_read called\n");
    (void)state;
    (void)ratio;
    (void)frames;
    (void)data;
    return 0;
}

__attribute__((weak)) const char *src_strerror(int error)
{
    switch_log("STUB: src_strerror called\n");
    (void)error;
    return "libsamplerate disabled";
}

__attribute__((weak)) int src_reset(SRC_STATE *state)
{
    switch_log("STUB: src_reset called\n");
    (void)state;
    return 0;
}

__attribute__((weak)) void src_float_to_short_array(const float *in, short *out, int len)
{
    switch_log("STUB: src_float_to_short_array called\n");
    (void)in;
    if (out && len > 0) {
        memset(out, 0, (size_t)len * sizeof(short));
    }
}

/* Note: audio_init_audiodevs, audio_create_default_audiodevs, and audio_define_default are now in QEMU core */
