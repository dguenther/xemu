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

int replay_mode = 0;

void replay_shutdown_request(int reason)
{
    (void)reason;
}

__attribute__((weak)) bool replay_events_enabled(void)
{
    return false;
}

void replay_configure(QemuOpts *opts)
{
    (void)opts;
}

void replay_audio_in(size_t *recorded, void *samples, size_t *wpos, size_t size)
{
    (void)recorded;
    (void)samples;
    (void)wpos;
    (void)size;
}

void replay_audio_out(size_t *played)
{
    (void)played;
}

void replay_mutex_lock(void)
{
}

void replay_mutex_unlock(void)
{
}

void replay_char_write_event_save(int res, int offset)
{
    (void)res;
    (void)offset;
}

void replay_char_write_event_load(int *res, int *offset)
{
    if (res) *res = 0;
    if (offset) *offset = 0;
}

void replay_vmstate_init(void)
{
}

void replay_start(void)
{
}

bool replay_checkpoint(int checkpoint)
{
    (void)checkpoint;
    return true;
}

void replay_finish(void)
{
}

void replay_add_blocker(const char *feature)
{
    (void)feature;
}

void replay_register_char_driver(void *chr)
{
    (void)chr;
}

int replay_running_debug(void)
{
    return 0;
}

int replay_breakpoint(void)
{
    return 0;
}

int replay_interrupt(void)
{
    return 0;
}

int replay_has_interrupt(void)
{
    return 0;
}

void replay_exception(void)
{
}

int replay_has_exception(void)
{
    return 0;
}

int replay_has_event(void)
{
    return 0;
}

void replay_async_events(void)
{
}

int64_t replay_get_instructions(void)
{
    return 0;
}

void replay_account_executed_instructions(void)
{
}

void replay_save_clock(int clock_id, int64_t value, int64_t raw_icount)
{
    (void)clock_id;
    (void)value;
    (void)raw_icount;
}

int64_t replay_read_clock(int clock_id, int64_t raw_icount)
{
    (void)clock_id;
    (void)raw_icount;
    return 0;
}

void replay_save_random(int ret, void *buf, size_t len)
{
    (void)ret;
    (void)buf;
    (void)len;
}

size_t replay_read_random(void *buf, size_t len)
{
    (void)buf;
    return len;
}

void replay_bh_schedule_event(void *bh)
{
    (void)bh;
}
