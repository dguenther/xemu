/*
 * QEMU Stubs for Nintendo Switch - Timer Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"
#include "qemu/timer.h"

/* Forward declarations */
typedef struct QEMUClock QEMUClock;

/*
 * Timer stubs
 */
typedef struct QEMUTimer QEMUTimer;

__attribute__((weak)) void timer_mod(QEMUTimer *ts, int64_t expire_time)
{
    (void)ts;
    (void)expire_time;
}

__attribute__((weak)) void timer_del(QEMUTimer *ts)
{
    (void)ts;
}

void configure_rtc(QemuOpts *opts)
{
    (void)opts;
}

/* Additional timer functions */
QEMUTimerListGroup timers_state = { 0 };

bool runstate_is_running(void)
{
    return true;
}

int64_t cpu_get_clock_locked(void)
{
    return 0;
}

bool all_cpu_threads_idle(void)
{
    return true;
}

void __attribute__((weak)) icount_start_warp_timer(void)
{
}

void __attribute__((weak)) icount_account_warp_timer(void)
{
}

void __attribute__((weak)) icount_notify_exit(void)
{
}

ICountMode icount_align_option = 0;

/* RTC clock */
QEMUClock *rtc_clock = NULL;

/* Time/date functions */
int64_t qemu_timedate_diff(struct tm *tm)
{
    (void)tm;
    return 0;
}

void qapi_event_send_rtc_change(int64_t offset)
{
    (void)offset;
}

void qemu_get_timedate(struct tm *tm, int offset)
{
    (void)tm; (void)offset;
}

int64_t cpu_get_clock(void)
{
    return 0;
}

int64_t cpus_get_virtual_clock(void)
{
    return 0;
}

int64_t __attribute__((weak)) icount_get_raw(void)
{
    return 0;
}
