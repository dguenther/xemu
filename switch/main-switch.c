/*
 * Nintendo Switch main entry point for xemu
 *
 * This file provides the main() function and initialization
 * for running xemu on Nintendo Switch with libnx.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "config-switch.h"
#include "platform_stubs.h"
#include "glib-compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  /* For mkdir */

#include <switch.h>

/* SDL2 for input handling */
#include <SDL.h>

/* EGL context helper */
#include "egl-switch.h"

/* xemu HUD interface */
#include "xui/xemu-hud.h"

/* xemu settings (populates g_config defaults and loads/saves TOML) */
#include "xemu-settings.h"

/* Boot request from UI */
extern int switch_consume_boot_bios_request(void);

/* Forward declarations */
extern void switch_platform_init(void);
extern void switch_platform_cleanup(void);
extern void switch_run_xemu_bios(void);
void switch_log(const char *format, ...);

/*
 * Switch-specific memory configuration
 * Homebrew applications can request more memory
 */
#ifdef __SWITCH__
/* Request extended memory mode for more RAM */
u32 __nx_applet_heap_size = 0x10000000;  /* 256MB for general heap */
u32 __nx_applet_exit_mode = 1;           /* Exit cleanly */
#endif

/*
 * Console output redirection
 * On Switch, we need to set up nxlink or file-based logging
 */
static FILE *log_file = NULL;

static int mkdir_path(const char *path)
{
#ifdef __SWITCH__
    return mkdir(path, 0755);
#else
    (void)path;
    return -1;
#endif
}

static void init_logging(void)
{
#ifdef __SWITCH__
    /* Try to open a log file on SD card */
    log_file = fopen("sdmc:/switch/xemu/xemu.log", "w");
    if (log_file) {
        setvbuf(log_file, NULL, _IOLBF, 0);
        return;
    }

    /* Fallback to default sdmc mount (relative paths) */
    log_file = fopen("/switch/xemu/xemu.log", "w");
    if (log_file) {
        setvbuf(log_file, NULL, _IOLBF, 0);
    }
#endif
}

static void cleanup_logging(void)
{
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

static int settings_ready = 0;

static void init_settings(void)
{
    /*
     * Avoid SDL_GetPrefPath()/portable-mode logic on Switch by setting an explicit path.
     * The main menu dereferences many g_config strings; without xemu_settings_load() those
     * pointers are NULL and the UI will crash as soon as it is drawn.
     */
    xemu_settings_set_path("sdmc:/switch/xemu/xemu.toml");
    settings_ready = 1;

    if (!xemu_settings_load()) {
        const char *err = xemu_settings_get_error_message();
        switch_log("Warning: failed to load settings, using defaults.\n");
        if (err && err[0]) {
            switch_log("%s\n", err);
        }
    } else {
        switch_log("Settings loaded\n");
    }
}

static void save_settings(void)
{
    if (!settings_ready) {
        return;
    }
    xemu_settings_save();
}

void switch_log(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    if (log_file) {
        va_list args_copy;
        va_copy(args_copy, args);
        vfprintf(log_file, format, args_copy);
        va_end(args_copy);
        fflush(log_file);
    }

    /* Also output to stderr for nxlink debugging */
    vfprintf(stderr, format, args);

    va_end(args);
}

/*
 * Create necessary directories on SD card
 */
static void ensure_directories(void)
{
#ifdef __SWITCH__
    /* Create xemu directory structure */
    mkdir_path("sdmc:/switch");
    mkdir_path("sdmc:/switch/xemu");
    mkdir_path("sdmc:/switch/xemu/data");
    mkdir_path("sdmc:/switch/xemu/saves");
    mkdir_path("sdmc:/switch/xemu/tmp");
    mkdir_path("sdmc:/switch/xemu/bios");
    mkdir_path("sdmc:/switch/xemu/discs");

    mkdir_path("/switch");
    mkdir_path("/switch/xemu");
    mkdir_path("/switch/xemu/data");
    mkdir_path("/switch/xemu/saves");
    mkdir_path("/switch/xemu/tmp");
    mkdir_path("/switch/xemu/bios");
    mkdir_path("/switch/xemu/discs");
#endif
}

/*
 * Initialize libnx services
 */
static int init_switch_services(void)
{
#ifdef __SWITCH__
    Result rc;

    /* Initialize applet services */
    rc = appletInitialize();
    if (R_FAILED(rc)) {
        switch_log("Failed to initialize applet: 0x%x\n", rc);
        return -1;
    }

    /* Filesystem/romfs are already set up for applications. */

    /* Initialize HID for controller input */
    hidInitialize();

    switch_log("Switch services initialized successfully\n");
#endif
    return 0;
}

/*
 * Cleanup libnx services
 */
static void cleanup_switch_services(void)
{
#ifdef __SWITCH__
    hidExit();
    appletExit();
#endif
}

/*
 * Build xemu command line arguments
 */
static char **build_xemu_args(int *argc_out)
{
    /* For now, use hardcoded arguments for initial testing */
    static char *argv[] = {
        "xemu",
        "-m", "64",                          /* 64MB RAM for Xbox */
        "-machine", "xbox",                  /* Xbox machine type */
        NULL
    };

    *argc_out = 5;
    return argv;
}

/*
 * Update ImGui gamepad input from Switch controller state
 * This function maps Switch controller buttons and sticks to ImGui's gamepad navigation
 */
static void update_imgui_gamepad_input(PadState *pad)
{
    extern void xemu_hud_update_gamepad_state(u64 buttons_down, u64 buttons_held,
                                              float lstick_x, float lstick_y,
                                              float rstick_x, float rstick_y);

    /* Update pad state */
    padUpdate(pad);

    /* Get button states */
    u64 kDown = padGetButtonsDown(pad);
    u64 kHeld = padGetButtons(pad);

    /* Get analog stick positions */
    HidAnalogStickState lstick = padGetStickPos(pad, 0);
    HidAnalogStickState rstick = padGetStickPos(pad, 1);

    /* Normalize stick values from [-32768, 32767] to [-1.0, 1.0] */
    float lstick_x = lstick.x / 32768.0f;
    float lstick_y = lstick.y / 32768.0f;
    float rstick_x = rstick.x / 32768.0f;
    float rstick_y = rstick.y / 32768.0f;

    /* Pass to HUD system */
    xemu_hud_update_gamepad_state(kDown, kHeld, lstick_x, lstick_y, rstick_x, rstick_y);
}

/*
 * Check for required files
 */
static int check_required_files(void)
{
    /* Check for MCPX ROM */
    if (!g_file_test("sdmc:/switch/xemu/bios/mcpx_1.0.bin", G_FILE_TEST_EXISTS)) {
        switch_log("Warning: MCPX ROM not found at sdmc:/switch/xemu/bios/mcpx_1.0.bin\n");
        switch_log("Please copy the MCPX ROM file to this location.\n");
        /* Don't fail - let xemu show its own error */
    }

    /* Check for BIOS */
    if (!g_file_test("sdmc:/switch/xemu/bios/bios.bin", G_FILE_TEST_EXISTS)) {
        switch_log("Warning: BIOS not found at sdmc:/switch/xemu/bios/bios.bin\n");
        switch_log("Please copy a compatible Xbox BIOS to this location.\n");
    }

    /* Check for hard disk image */
    if (!g_file_test("sdmc:/switch/xemu/xbox_hdd.qcow2", G_FILE_TEST_EXISTS)) {
        switch_log("Warning: HDD image not found at sdmc:/switch/xemu/xbox_hdd.qcow2\n");
        switch_log("Please create or copy an Xbox HDD image to this location.\n");
    }

    return 0;
}

/*
 * Main entry point
 */
int main(int argc, char **argv)
{
#ifdef __SWITCH__
    int ret = 0;
    int console_active = 0;
    SDL_Window *window = NULL;

    (void)argc;
    (void)argv;

    /* Show console briefly for boot message */
    consoleInit(NULL);
    console_active = 1;
    printf("xemu for Nintendo Switch\n");
    printf("Initializing...\n");

    /* Configure controller input */
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    /* Initialize Switch services */
    if (init_switch_services() != 0) {
        printf("Failed to initialize Switch services\n");
        goto cleanup;
    }

    /* Initialize networking for nxlink debugging */
    Result sock_rc = socketInitializeDefault();
    if (R_SUCCEEDED(sock_rc)) {
        int nxlink_sock = nxlinkStdio();
        if (nxlink_sock >= 0) {
            setvbuf(stdout, NULL, _IONBF, 0);
            setvbuf(stderr, NULL, _IONBF, 0);
            switch_log("nxlink attached\n");
        }
    }

    /* Initialize platform abstraction */
    switch_platform_init();

    /* Create directories and set up logging */
    ensure_directories();
    init_logging();
    switch_log("xemu for Nintendo Switch starting...\n");

    /* Load settings (populates g_config defaults) before initializing the HUD/main menu. */
    init_settings();

    /* Console and GPU output cannot be used together; shut down console before EGL. */
    if (console_active) {
        consoleExit(NULL);
        console_active = 0;
    }

    /* Initialize EGL with OpenGL 4.3 Core context */
    switch_log("Initializing EGL...\n");
    if (!switch_egl_init()) {
        switch_log("Failed to initialize EGL\n");
        ret = 1;
        goto cleanup;
    }
    switch_log("EGL initialized with OpenGL 4.3 Core\n");

    /* Initialize SDL for input handling only - no video needed since we use EGL */
    switch_log("Initializing SDL2 for input...\n");
    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        switch_log("SDL_Init failed: %s\n", SDL_GetError());
        ret = 1;
        goto cleanup_egl;
    }
    switch_log("SDL2 initialized (joystick/gamecontroller only)\n");

    /* On Switch, we don't create an SDL window since EGL owns the NWindow.
     * SDL is only used for input handling via joystick/gamecontroller APIs.
     * We'll create a dummy window pointer for ImGui compatibility. */
    switch_log("Skipping SDL window creation (EGL owns display)\n");
    window = NULL;  /* No SDL window - EGL handles display */

    /* Flush output before HUD init in case it crashes */
    fflush(stdout);
    fflush(stderr);

    /* Initialize the xemu HUD with SDL window and EGL context */
    switch_log("Initializing xemu HUD...\n");
    fflush(stderr);
    xemu_hud_init(window, switch_egl_get_context());
    switch_log("xemu HUD initialized\n");
    fflush(stderr);

    /* Main loop */
    switch_log("Entering main loop\n");
    fflush(stderr);

    static int frame_count = 0;
    int running = 1;
    int boot_requested = 0;
    while (running && appletMainLoop()) {
        if (frame_count < 5 || frame_count % 60 == 0) {
            switch_log("Frame %d\n", frame_count);
            fflush(stderr);
        }
        frame_count++;

        /* Update gamepad state for ImGui navigation */
        update_imgui_gamepad_input(&pad);

        /* Check for Plus button to exit (from updated pad state) */
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) {
            running = 0;
        }

        /* Process SDL events (mostly for compatibility, but not critical on Switch) */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            /* Pass events to HUD for ImGui processing */
            xemu_hud_process_sdl_events(&event);

            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        /* Render the HUD */
        if (frame_count <= 3) {
            switch_log("Frame %d: calling xemu_hud_render...\n", frame_count - 1);
            fflush(stderr);
        }
        xemu_hud_render();
        if (frame_count <= 3) {
            switch_log("Frame %d: xemu_hud_render done, calling swap...\n", frame_count - 1);
            fflush(stderr);
        }

        /* Swap EGL buffers */
        switch_egl_swap();
        if (frame_count <= 3) {
            switch_log("Frame %d: swap done\n", frame_count - 1);
            fflush(stderr);
        }

        if (switch_consume_boot_bios_request()) {
            switch_log("Boot requested: starting Xbox BIOS...\n");
            boot_requested = 1;
            break;
        }
    }

    switch_log("Exiting main loop\n");

    /* If a boot was requested, keep EGL + HUD alive and enter the emulator loop. */
    if (boot_requested) {
        switch_run_xemu_bios();
    }

    /* Cleanup HUD */
    xemu_hud_cleanup();

    if (window) {
        SDL_DestroyWindow(window);
    }
cleanup_sdl:
    SDL_Quit();
cleanup_egl:
    switch_egl_cleanup();
cleanup:
    save_settings();
    switch_platform_cleanup();
    cleanup_switch_services();
    cleanup_logging();
    if (R_SUCCEEDED(sock_rc)) {
        socketExit();
    }
    if (console_active) {
        consoleExit(NULL);
    }

    return ret;
#else
    (void)argc;
    (void)argv;
    return 0;
#endif
}

/*
 * Error handling helpers
 */
void switch_fatal_error(const char *msg)
{
    switch_log("FATAL ERROR: %s\n", msg);

#ifdef __SWITCH__
    /* Show error dialog on Switch */
    ErrorApplicationConfig cfg;
    errorApplicationCreate(&cfg, msg, NULL);
    errorApplicationShow(&cfg);
#endif

    exit(1);
}

/*
 * Performance helpers
 */
void switch_boost_cpu(int enable)
{
#ifdef __SWITCH__
    (void)enable;
#else
    (void)enable;
#endif
}

/*
 * Get available memory
 */
size_t switch_get_available_memory(void)
{
#ifdef __SWITCH__
    u64 available = 0;
    svcGetInfo(&available, InfoType_TotalMemorySize, CUR_PROCESS_HANDLE, 0);
    return (size_t)available;
#else
    return 3ULL * 1024 * 1024 * 1024; /* 3GB default */
#endif
}

/*
 * Check if running in docked mode
 */
int switch_is_docked(void)
{
#ifdef __SWITCH__
    return appletGetOperationMode() == AppletOperationMode_Console;
#else
    return 1;
#endif
}
