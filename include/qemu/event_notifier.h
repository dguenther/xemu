/*
 * event notifier support
 *
 * Copyright Red Hat, Inc. 2010
 *
 * Authors:
 *  Michael S. Tsirkin <mst@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef QEMU_EVENT_NOTIFIER_H
#define QEMU_EVENT_NOTIFIER_H


#ifdef _WIN32
#include <windows.h>
#endif

#ifdef CONFIG_SWITCH
/* Forward-declare libnx LEvent; switch-specific compilation units include the
 * real header (<switch/kernel/levent.h>) before this file when they need it.
 */
typedef struct LEvent LEvent;
#endif

typedef struct EventNotifier {
#ifdef _WIN32
    HANDLE event;
#elif defined(CONFIG_SWITCH)
    /* Switch uses libnx LEvent for proper kernel wake handling. We store a
     * heap-allocated or otherwise externally-managed LEvent pointer here so
     * we don't need to include libnx headers in every consumer of this
     * header (which would cause name collisions).
     */
    LEvent *levent;
    bool initialized;
#else
    int rfd;
    int wfd;
    bool initialized;
#endif
} EventNotifier;

typedef void EventNotifierHandler(EventNotifier *);

int event_notifier_init(EventNotifier *, int active);
void event_notifier_cleanup(EventNotifier *);
int event_notifier_set(EventNotifier *);
int event_notifier_test_and_clear(EventNotifier *);

#ifdef CONFIG_POSIX
void event_notifier_init_fd(EventNotifier *, int fd);
int event_notifier_get_fd(const EventNotifier *);
int event_notifier_get_wfd(const EventNotifier *);
#elif defined(_WIN32)
HANDLE event_notifier_get_handle(EventNotifier *);
#endif

#endif
