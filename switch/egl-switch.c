/*
 * Nintendo Switch EGL Context Helper
 *
 * Provides OpenGL 4.3 Core context creation using EGL on Switch.
 * Based on libnx examples.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "egl-switch.h"

#ifdef __SWITCH__

#include <stdio.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <glad/glad.h>

/* Logging function from main-switch.c */
extern void switch_log(const char *format, ...);

/* EGL state */
static EGLDisplay s_display = EGL_NO_DISPLAY;
static EGLContext s_context = EGL_NO_CONTEXT;
static EGLSurface s_surface = EGL_NO_SURFACE;
static int s_width = 1280;
static int s_height = 720;

bool switch_egl_init(void)
{
    /* Set Mesa configuration for better debugging */
    /* setenv("MESA_NO_ERROR", "1", 1); */  /* Disable error checking for performance */

    /* Connect to the EGL default display */
    s_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (s_display == EGL_NO_DISPLAY) {
        switch_log("EGL: Could not connect to display! error: 0x%x\n", eglGetError());
        return false;
    }

    /* Initialize the EGL display connection */
    EGLint major, minor;
    if (!eglInitialize(s_display, &major, &minor)) {
        switch_log("EGL: Failed to initialize! error: 0x%x\n", eglGetError());
        goto fail_display;
    }
    switch_log("EGL: Initialized version %d.%d\n", major, minor);

    /* Select OpenGL (Core) as the desired graphics API */
    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE) {
        switch_log("EGL: Could not set OpenGL API! error: 0x%x\n", eglGetError());
        goto fail_init;
    }
    switch_log("EGL: Bound OpenGL API\n");

    /* Get an appropriate EGL framebuffer configuration */
    static const EGLint framebufferAttributeList[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    eglChooseConfig(s_display, framebufferAttributeList, &config, 1, &numConfigs);
    if (numConfigs == 0) {
        switch_log("EGL: No config found! error: 0x%x\n", eglGetError());
        goto fail_init;
    }
    switch_log("EGL: Found %d config(s)\n", numConfigs);

    /* Create an EGL window surface using the default NWindow */
    NWindow *win = nwindowGetDefault();
    if (!win) {
        switch_log("EGL: Could not get default NWindow!\n");
        goto fail_init;
    }

    /* Set window dimensions - required before creating EGL surface */
    /* Use 1280x720 for docked/handheld compatibility */
    s_width = 1280;
    s_height = 720;
    nwindowSetDimensions(win, s_width, s_height);
    switch_log("EGL: Set NWindow dimensions to %dx%d\n", s_width, s_height);

    s_surface = eglCreateWindowSurface(s_display, config, (EGLNativeWindowType)win, NULL);
    if (s_surface == EGL_NO_SURFACE) {
        switch_log("EGL: Surface creation failed! error: 0x%x\n", eglGetError());
        goto fail_init;
    }
    switch_log("EGL: Created window surface\n");

    /* Create an EGL rendering context with OpenGL 4.3 Core */
    static const EGLint contextAttributeList[] = {
        EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
        EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
        EGL_CONTEXT_MINOR_VERSION_KHR, 3,
        EGL_NONE
    };

    s_context = eglCreateContext(s_display, config, EGL_NO_CONTEXT, contextAttributeList);
    if (s_context == EGL_NO_CONTEXT) {
        switch_log("EGL: Context creation failed! error: 0x%x\n", eglGetError());
        goto fail_surface;
    }
    switch_log("EGL: Created OpenGL 4.3 Core context\n");

    /* Connect the context to the surface */
    if (!eglMakeCurrent(s_display, s_surface, s_surface, s_context)) {
        switch_log("EGL: Failed to make context current! error: 0x%x\n", eglGetError());
        goto fail_context;
    }
    switch_log("EGL: Made context current\n");

    /* Load OpenGL functions using glad */
    if (!gladLoadGL()) {
        switch_log("EGL: Failed to load OpenGL functions via glad\n");
        goto fail_context;
    }
    switch_log("EGL: Loaded OpenGL %d.%d via glad\n", GLVersion.major, GLVersion.minor);

    /* Query surface size - only use if valid, otherwise keep our set dimensions */
    EGLint query_w = 0, query_h = 0;
    eglQuerySurface(s_display, s_surface, EGL_WIDTH, &query_w);
    eglQuerySurface(s_display, s_surface, EGL_HEIGHT, &query_h);
    if (query_w > 0 && query_h > 0) {
        s_width = query_w;
        s_height = query_h;
    }
    switch_log("EGL: Surface size: %dx%d\n", s_width, s_height);

    /* Set initial viewport */
    glViewport(0, 0, s_width, s_height);

    return true;

fail_context:
    eglDestroyContext(s_display, s_context);
    s_context = EGL_NO_CONTEXT;
fail_surface:
    eglDestroySurface(s_display, s_surface);
    s_surface = EGL_NO_SURFACE;
fail_init:
    eglTerminate(s_display);
fail_display:
    s_display = EGL_NO_DISPLAY;
    return false;
}

void switch_egl_swap(void)
{
    if (s_display != EGL_NO_DISPLAY && s_surface != EGL_NO_SURFACE) {
        eglSwapBuffers(s_display, s_surface);
    }
}

void *switch_egl_get_context(void)
{
    return (void *)s_context;
}

int switch_egl_get_width(void)
{
    return s_width;
}

int switch_egl_get_height(void)
{
    return s_height;
}

void switch_egl_cleanup(void)
{
    if (s_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(s_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (s_context != EGL_NO_CONTEXT) {
            eglDestroyContext(s_display, s_context);
            s_context = EGL_NO_CONTEXT;
        }
        if (s_surface != EGL_NO_SURFACE) {
            eglDestroySurface(s_display, s_surface);
            s_surface = EGL_NO_SURFACE;
        }
        eglTerminate(s_display);
        s_display = EGL_NO_DISPLAY;
    }
    switch_log("EGL: Cleaned up\n");
}

#else /* !__SWITCH__ */

#include <stddef.h>  /* For NULL */

/* Stub implementation for non-Switch builds */

bool switch_egl_init(void)
{
    return false;
}

void switch_egl_swap(void)
{
}

void *switch_egl_get_context(void)
{
    return NULL;
}

int switch_egl_get_width(void)
{
    return 1280;
}

int switch_egl_get_height(void)
{
    return 720;
}

void switch_egl_cleanup(void)
{
}

#endif /* __SWITCH__ */
