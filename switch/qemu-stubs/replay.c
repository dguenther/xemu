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
    switch_log("STUB: replay_shutdown_request called\n");
    (void)reason;
}

__attribute__((weak)) bool replay_events_enabled(void)
{
    switch_log("STUB: replay_events_enabled called\n");
    return false;
}

void replay_configure(QemuOpts *opts)
{
    switch_log("STUB: replay_configure called\n");
    (void)opts;
}

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

void replay_mutex_lock(void)
{
    switch_log("STUB: replay_mutex_lock called\n");
}

void replay_mutex_unlock(void)
{
    switch_log("STUB: replay_mutex_unlock called\n");
}

void replay_char_write_event_save(int res, int offset)
{
    switch_log("STUB: replay_char_write_event_save called\n");
    (void)res;
    (void)offset;
}

void replay_char_write_event_load(int *res, int *offset)
{
    switch_log("STUB: replay_char_write_event_load called\n");
    if (res) *res = 0;
    if (offset) *offset = 0;
}

void replay_vmstate_init(void)
{
    switch_log("STUB: replay_vmstate_init called\n");
}

void replay_start(void)
{
    switch_log("STUB: replay_start called\n");
}

bool replay_checkpoint(int checkpoint)
{
    switch_log("STUB: replay_checkpoint called\n");
    (void)checkpoint;
    return true;
}

void replay_finish(void)
{
    switch_log("STUB: replay_finish called\n");
}

void replay_add_blocker(const char *feature)
{
    switch_log("STUB: replay_add_blocker called\n");
    (void)feature;
}

void replay_register_char_driver(void *chr)
{
    switch_log("STUB: replay_register_char_driver called\n");
    (void)chr;
}

void replay_chr_be_write(struct Chardev *s, const uint8_t *buf, int len)
{
    switch_log("STUB: replay_chr_be_write called\n");
    (void)s;
    (void)buf;
    (void)len;
}

int replay_running_debug(void)
{
    switch_log("STUB: replay_running_debug called\n");
    return 0;
}

int replay_breakpoint(void)
{
    switch_log("STUB: replay_breakpoint called\n");
    return 0;
}

int replay_interrupt(void)
{
    switch_log("STUB: replay_interrupt called\n");
    return 0;
}

int replay_has_interrupt(void)
{
    switch_log("STUB: replay_has_interrupt called\n");
    return 0;
}

void replay_exception(void)
{
    switch_log("STUB: replay_exception called\n");
}

int replay_has_exception(void)
{
    switch_log("STUB: replay_has_exception called\n");
    return 0;
}

int replay_has_event(void)
{
    switch_log("STUB: replay_has_event called\n");
    return 0;
}

void replay_async_events(void)
{
    switch_log("STUB: replay_async_events called\n");
}

void replay_input_event(void *event)
{
    switch_log("STUB: replay_input_event called\n");
    (void)event;
}

void replay_input_sync_event(void)
{
    switch_log("STUB: replay_input_sync_event called\n");
}

int64_t replay_get_instructions(void)
{
    switch_log("STUB: replay_get_instructions called\n");
    return 0;
}

void replay_account_executed_instructions(void)
{
    switch_log("STUB: replay_account_executed_instructions called\n");
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

void replay_bh_schedule_event(void *bh)
{
    switch_log("STUB: replay_bh_schedule_event called\n");
    (void)bh;
}
