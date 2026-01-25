/*
 * event notifier support - Nintendo Switch implementation
 *
 * Uses libnx LEvent (Light Event) for synchronization primitives.
 * LEvent provides proper SVC-based thread waking via svcSignalToAddress.
 *
 * Copyright (c) 2024-2026 xemu contributors
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "config-switch.h"

/* libnx headers FIRST to own the namespace */
#include <switch/kernel/levent.h>

/* Minimal standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* EventNotifier struct definition */
#include "qemu/event_notifier.h"

int event_notifier_init(EventNotifier *e, int active)
{
    e->levent = malloc(sizeof(LEvent));
    if (!e->levent) {
        return -1;
    }
    leventInit(e->levent, active != 0, true);
    e->initialized = true;
    return 0;
}

void event_notifier_cleanup(EventNotifier *e)
{
    if (!e->initialized) {
        return;
    }
    /* LEvent has no kernel resources to free, but we allocated memory for it */
    free(e->levent);
    e->levent = NULL;
    e->initialized = false;
}

int event_notifier_set(EventNotifier *e)
{
    if (!e->initialized) {
        return -1;
    }
    leventSignal(e->levent);
    return 0;
}

int event_notifier_test_and_clear(EventNotifier *e)
{
    if (!e->initialized) {
        return 0;
    }
    return leventTryWait(e->levent) ? 1 : 0;
}
