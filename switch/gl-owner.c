/*
 * Switch GL ownership synchronization
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "gl-owner.h"

#include <pthread.h>

static pthread_once_t g_gl_owner_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_gl_owner_mutex;

static void switch_gl_owner_init(void)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&g_gl_owner_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

void switch_gl_owner_lock(const char *tag)
{
    (void)tag;
    pthread_once(&g_gl_owner_once, switch_gl_owner_init);
    pthread_mutex_lock(&g_gl_owner_mutex);
}

void switch_gl_owner_unlock(const char *tag)
{
    (void)tag;
    pthread_mutex_unlock(&g_gl_owner_mutex);
}

bool switch_gl_owner_trylock(const char *tag)
{
    (void)tag;
    pthread_once(&g_gl_owner_once, switch_gl_owner_init);
    return pthread_mutex_trylock(&g_gl_owner_mutex) == 0;
}
