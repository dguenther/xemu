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

/* These are always needed */
void __attribute__((weak)) icount_start_warp_timer(void)
{
}

void __attribute__((weak)) icount_account_warp_timer(void)
{
}

void __attribute__((weak)) icount_notify_exit(void)
{
}

void qapi_event_send_rtc_change(int64_t offset)
{
    (void)offset;
}

int64_t __attribute__((weak)) icount_get_raw(void)
{
    return 0;
}
