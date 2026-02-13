/*
 * QEMU Stubs for Nintendo Switch - Replay/Record System
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Replay/record stubs
 */

void replay_audio_in(size_t *recorded, void *samples, size_t *wpos, size_t size)
{
    switch_log("STUB: replay_audio_in called\n");
    (void)recorded;
    (void)samples;
    (void)wpos;
    (void)size;
}

void replay_audio_out(size_t *played)
{
    switch_log("STUB: replay_audio_out called\n");
    (void)played;
}

void replay_save_clock(int clock_id, int64_t value, int64_t raw_icount)
{
    switch_log("STUB: replay_save_clock called\n");
    (void)clock_id;
    (void)value;
    (void)raw_icount;
}

int64_t replay_read_clock(int clock_id, int64_t raw_icount)
{
    switch_log("STUB: replay_read_clock called\n");
    (void)clock_id;
    (void)raw_icount;
    return 0;
}

void replay_save_random(int ret, void *buf, size_t len)
{
    switch_log("STUB: replay_save_random called\n");
    (void)ret;
    (void)buf;
    (void)len;
}

size_t replay_read_random(void *buf, size_t len)
{
    switch_log("STUB: replay_read_random called\n");
    (void)buf;
    return len;
}

void replay_event_net_run(void *opaque)
{
    switch_log("STUB: replay_event_net_run called\n");
}

void replay_event_net_save(void *opaque)
{
    switch_log("STUB: replay_event_net_save called\n");
}

void replay_event_net_load(void *opaque)
{
    switch_log("STUB: replay_event_net_load called\n");
}
