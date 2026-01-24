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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  /* For mkdir/stat */
#include <pthread.h>

#include <switch.h>

/* SDL2 for window/input handling */
#include <SDL.h>
#include <glad/glad.h>

/* xemu HUD interface */
#include "xui/xemu-hud.h"

/* QEMU main entrypoint */
#include "qemu-main.h"

/* xemu display (SDL window handoff) */
extern void xemu_switch_set_sdl_window(SDL_Window *window, SDL_GLContext context);

/* xemu settings (populates g_config defaults and loads/saves TOML) */
#include "xemu-settings.h"

/* Boot request from UI */
extern int switch_consume_boot_bios_request(void);

/* Forward declarations */
extern void switch_platform_init(void);
extern void switch_platform_cleanup(void);
extern void qemu_init(int argc, char **argv);
extern void switch_set_main_thread(pthread_t t);
void switch_log(const char *format, ...);

/* Userland exception handler to avoid hard lock and capture a dump. */
__attribute__((aligned(16))) u8 __nx_exception_stack[0x1000];
u64 __nx_exception_stack_size = sizeof(__nx_exception_stack);

void __libnx_exception_handler(ThreadExceptionDump *ctx)
{
    FILE *f = fopen("sdmc:/switch/xemu/exception_dump.txt", "w");
    if (!f) {
        f = fopen("/switch/xemu/exception_dump.txt", "w");
    }
    if (!f) {
        return;
    }

    fprintf(f, "error_desc: 0x%x\n", ctx->error_desc);
    for (int i = 0; i < 29; i++) {
        fprintf(f, "[X%d]: 0x%lx\n", i, ctx->cpu_gprs[i].x);
    }
    fprintf(f, "fp: 0x%lx\n", ctx->fp.x);
    fprintf(f, "lr: 0x%lx\n", ctx->lr.x);
    fprintf(f, "sp: 0x%lx\n", ctx->sp.x);
    fprintf(f, "pc: 0x%lx\n", ctx->pc.x);
    fprintf(f, "pstate: 0x%x\n", ctx->pstate);
    fprintf(f, "afsr0: 0x%x\n", ctx->afsr0);
    fprintf(f, "afsr1: 0x%x\n", ctx->afsr1);
    fprintf(f, "esr: 0x%x\n", ctx->esr);
    fprintf(f, "far: 0x%lx\n", ctx->far.x);

    MemoryInfo mem_info;
    u32 page_info = 0;
    if (R_SUCCEEDED(svcQueryMemory(&mem_info, &page_info, ctx->lr.x))) {
        fprintf(f, "lr region: base=0x%lx size=0x%lx type=0x%x perm=0x%x attr=0x%x\n",
                mem_info.addr, mem_info.size, mem_info.type, mem_info.perm, mem_info.attr);
        fprintf(f, "lr offset: 0x%lx\n", ctx->lr.x - mem_info.addr);
    }
    if (R_SUCCEEDED(svcQueryMemory(&mem_info, &page_info, ctx->pc.x))) {
        fprintf(f, "pc region: base=0x%lx size=0x%lx type=0x%x perm=0x%x attr=0x%x\n",
                mem_info.addr, mem_info.size, mem_info.type, mem_info.perm, mem_info.attr);
        fprintf(f, "pc offset: 0x%lx\n", ctx->pc.x - mem_info.addr);
    }

    fclose(f);
}

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
static int log_to_stderr = 0;
static int nxlink_active = 0;
static int console_active = 0;

static void switch_update_stderr_state(void)
{
    log_to_stderr = console_active || nxlink_active;
}

int switch_log_stderr_enabled(void)
{
    return log_to_stderr;
}

static int mkdir_path(const char *path)
{
#ifdef __SWITCH__
    return mkdir(path, 0755);
#else
    (void)path;
    return -1;
#endif
}

static int file_exists(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0;
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

    /* Disable welcome screen on Switch - we want to boot immediately with defaults */
    g_config.general.show_welcome = false;

    /* Set Switch-specific default paths if not configured */
    if (!g_config.sys.files.flashrom_path || !g_config.sys.files.flashrom_path[0]) {
        g_free(g_config.sys.files.flashrom_path);
        g_config.sys.files.flashrom_path = g_strdup("sdmc:/switch/xemu/bios/bios.bin");
        switch_log("Using default BIOS path: %s\n", g_config.sys.files.flashrom_path);
    }

    if (!g_config.sys.files.bootrom_path || !g_config.sys.files.bootrom_path[0]) {
        g_free(g_config.sys.files.bootrom_path);
        g_config.sys.files.bootrom_path = g_strdup("sdmc:/switch/xemu/bios/mcpx_1.0.bin");
        switch_log("Using default MCPX ROM path: %s\n", g_config.sys.files.bootrom_path);
    }

    if (!g_config.sys.files.hdd_path || !g_config.sys.files.hdd_path[0]) {
        g_free(g_config.sys.files.hdd_path);
        g_config.sys.files.hdd_path = g_strdup("sdmc:/switch/xemu/bios/xbox_hdd.qcow2");
        switch_log("Using default HDD path: %s\n", g_config.sys.files.hdd_path);
    }

    if (!g_config.sys.files.eeprom_path || !g_config.sys.files.eeprom_path[0]) {
        g_free(g_config.sys.files.eeprom_path);
        g_config.sys.files.eeprom_path = g_strdup("sdmc:/switch/xemu/bios/eeprom.bin");
        switch_log("Using default EEPROM path: %s\n", g_config.sys.files.eeprom_path);
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
    if (log_to_stderr) {
        vfprintf(stderr, format, args);
    }

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
        "-display", "xemu",               /* Use xemu SDL+OpenGL display */
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
    int ok = 1;

    const char *mcpx_path = g_config.sys.files.bootrom_path;
    const char *bios_path = g_config.sys.files.flashrom_path;
    const char *hdd_path = g_config.sys.files.hdd_path;

    /* Check for MCPX ROM */
    if (!mcpx_path || !mcpx_path[0]) {
        switch_log("Warning: MCPX ROM path not set in settings.\n");
    } else if (!file_exists(mcpx_path)) {
        switch_log("Warning: MCPX ROM not found at %s\n", mcpx_path);
        switch_log("Please copy the MCPX ROM file to this location.\n");
        /* Don't fail - let xemu show its own error */
    }

    /* Check for BIOS */
    if (!bios_path || !bios_path[0]) {
        switch_log("Warning: BIOS path not set in settings.\n");
        ok = 0;
    } else if (!file_exists(bios_path)) {
        switch_log("Warning: BIOS not found at %s\n", bios_path);
        switch_log("Please copy a compatible Xbox BIOS to this location.\n");
        ok = 0;
    }

    /* Check for hard disk image */
    if (!hdd_path || !hdd_path[0]) {
        switch_log("Warning: HDD path not set in settings.\n");
        ok = 0;
    } else if (!file_exists(hdd_path)) {
        switch_log("Warning: HDD image not found at %s\n", hdd_path);
        switch_log("Please create or copy an Xbox HDD image to this location.\n");
        ok = 0;
    }

    return ok ? 0 : -1;
}

static pthread_t qemu_thread;
static int qemu_thread_started = 0;

static void switch_qemu_thread_cleanup(void *opaque)
{
    (void)opaque;
    qemu_thread_started = 0;
    switch_log("Switch: QEMU thread exited\n");
}

static void *switch_qemu_thread_main(void *opaque)
{
    (void)opaque;

    pthread_cleanup_push(switch_qemu_thread_cleanup, NULL);

    int qemu_argc = 0;
    char **qemu_argv = build_xemu_args(&qemu_argc);

    switch_log("Switch: calling qemu_init (argc=%d)\n", qemu_argc);
    qemu_init(qemu_argc, qemu_argv);

    switch_log("Switch: entering qemu_main\n");
    int status = qemu_main();
    switch_log("Switch: qemu_main exited (%d)\n", status);
    pthread_cleanup_pop(1);
    return NULL;
}

static void switch_run_xemu_bios(void)
{
    if (qemu_thread_started) {
        switch_log("Boot already in progress\n");
        return;
    }

    if (check_required_files() != 0) {
        switch_log("Boot aborted: missing required BIOS/HDD files.\n");
        return;
    }

    qemu_thread_started = 1;
    int rc = pthread_create(&qemu_thread, NULL, switch_qemu_thread_main, NULL);
    if (rc != 0) {
        qemu_thread_started = 0;
        switch_log("Failed to start QEMU thread (%d)\n", rc);
        return;
    }
    pthread_detach(qemu_thread);
}

/*
 * Main entry point
 */
int main(int argc, char **argv)
{
#ifdef __SWITCH__
    int ret = 0;
    SDL_Window *window = NULL;
    SDL_GLContext gl_context = NULL;

    (void)argc;
    (void)argv;

    switch_set_main_thread(pthread_self());

    /* Show console briefly for boot message */
    consoleInit(NULL);
    console_active = 1;
    switch_update_stderr_state();
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
            nxlink_active = 1;
            switch_update_stderr_state();
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
        switch_update_stderr_state();
    }

    /* Initialize SDL for video + input and create a GL 4.3 core context */
    switch_log("Initializing SDL2 (video + input)...\n");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        switch_log("SDL_Init failed: %s\n", SDL_GetError());
        ret = 1;
        goto cleanup;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    window = SDL_CreateWindow("xemu",
                              0, 0, 1280, 720,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        switch_log("SDL_CreateWindow failed: %s\n", SDL_GetError());
        ret = 1;
        goto cleanup_sdl;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        switch_log("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        ret = 1;
        goto cleanup_sdl;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    if (!gladLoadGL()) {
        switch_log("gladLoadGL failed\n");
        ret = 1;
        goto cleanup_sdl;
    }

    xemu_switch_set_sdl_window(window, gl_context);

    SDL_GL_SetSwapInterval(1);
    switch_log("SDL2 GL context initialized (4.3 core)\n");

    /* Flush output before HUD init in case it crashes */
    fflush(stdout);
    if (switch_log_stderr_enabled()) {
        fflush(stderr);
    }

    /* Initialize the xemu HUD with SDL window and GL context */
    switch_log("Initializing xemu HUD...\n");
    if (switch_log_stderr_enabled()) {
        fflush(stderr);
    }
    xemu_hud_init(window, gl_context);
    switch_log("xemu HUD initialized\n");
    if (switch_log_stderr_enabled()) {
        fflush(stderr);
    }

    /* Main loop */
    switch_log("Entering main loop\n");
    if (switch_log_stderr_enabled()) {
        fflush(stderr);
    }

    static int frame_count = 0;
    int running = 1;
    while (running && appletMainLoop()) {
        if (frame_count < 5 || frame_count % 60 == 0) {
            switch_log("Frame %d\n", frame_count);
            if (switch_log_stderr_enabled()) {
                fflush(stderr);
            }
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
            if (switch_log_stderr_enabled()) {
                fflush(stderr);
            }
        }
        xemu_hud_render();
        if (frame_count <= 3) {
            switch_log("Frame %d: xemu_hud_render done, calling swap...\n", frame_count - 1);
            if (switch_log_stderr_enabled()) {
                fflush(stderr);
            }
        }

        /* Swap SDL GL buffers */
        SDL_GL_SwapWindow(window);
        if (frame_count <= 3) {
            switch_log("Frame %d: swap done\n", frame_count - 1);
            if (switch_log_stderr_enabled()) {
                fflush(stderr);
            }
        }

        if (switch_consume_boot_bios_request()) {
            switch_log("Boot requested: starting Xbox BIOS...\n");
            switch_run_xemu_bios();
        }
    }

    switch_log("Exiting main loop\n");

    /* Cleanup HUD */
    xemu_hud_cleanup();

    if (gl_context) {
        SDL_GL_DeleteContext(gl_context);
        gl_context = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
cleanup_sdl:
    SDL_Quit();
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
        console_active = 0;
        switch_update_stderr_state();
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
