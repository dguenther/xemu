//
// xemu User Interface
//
// Copyright (C) 2020-2022 Matt Borgerson
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <SDL.h>
// OpenGL loader abstraction: use glad on Switch, epoxy elsewhere
#ifdef CONFIG_SWITCH
#include <glad/glad.h>
#else
#include <epoxy/gl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <functional>
#include <assert.h>
#include <fpng.h>

#include <deque>
#include <vector>
#include <string>
#include <memory>

#include "actions.hh"
#include "common.hh"
#include "xemu-hud.h"
#include "misc.hh"
#include "gl-helpers.hh"
#include "input-manager.hh"
#include "snapshot-manager.hh"
#include "viewport-manager.hh"
#include "font-manager.hh"
#include "scene.hh"
#include "scene-manager.hh"
#include "main-menu.hh"
#include "popup-menu.hh"
#include "notifications.hh"
#include "monitor.hh"
#include "debug.hh"
#include "welcome.hh"
#include "menubar.hh"
#include "compat.hh"
#if defined(_WIN32)
#include "update.hh"
#endif

bool g_screenshot_pending;
const char *g_snapshot_pending_load_name;

float g_main_menu_height;

static ImGuiStyle g_base_style;
static SDL_Window *g_sdl_window;
static float g_last_scale;
static int g_vsync;
static GLuint g_tex;
static bool g_flip_req;

#ifdef CONFIG_SWITCH
static int g_switch_boot_bios_requested = 0;

extern "C" void switch_request_boot_bios(void)
{
    g_switch_boot_bios_requested = 1;
}

extern "C" int switch_consume_boot_bios_request(void)
{
    int v = g_switch_boot_bios_requested;
    g_switch_boot_bios_requested = 0;
    return v;
}
#endif


static void InitializeStyle()
{
    g_font_mgr.Rebuild();

    ImGui::StyleColorsDark();
    ImVec4 *c = ImGui::GetStyle().Colors;
    c[ImGuiCol_Text]                  = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    c[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    c[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    c[ImGuiCol_ChildBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.98f);
    c[ImGuiCol_PopupBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    c[ImGuiCol_Border]                = ImVec4(0.11f, 0.11f, 0.11f, 0.60f);
    c[ImGuiCol_BorderShadow]          = ImVec4(0.16f, 0.16f, 0.16f, 0.00f);
    c[ImGuiCol_FrameBg]               = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    c[ImGuiCol_FrameBgHovered]        = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    c[ImGuiCol_FrameBgActive]         = ImVec4(0.28f, 0.71f, 0.25f, 1.00f);
    c[ImGuiCol_TitleBg]               = ImVec4(0.20f, 0.51f, 0.18f, 1.00f);
    c[ImGuiCol_TitleBgActive]         = ImVec4(0.26f, 0.66f, 0.23f, 1.00f);
    c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.16f, 0.16f, 0.16f, 0.75f);
    c[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 0.00f);
    c[ImGuiCol_ScrollbarBg]           = ImVec4(0.16f, 0.16f, 0.16f, 0.00f);
    c[ImGuiCol_ScrollbarGrab]         = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    c[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_CheckMark]             = ImVec4(0.26f, 0.66f, 0.23f, 1.00f);
    c[ImGuiCol_SliderGrab]            = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    c[ImGuiCol_SliderGrabActive]      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    c[ImGuiCol_Button]                = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
    c[ImGuiCol_ButtonHovered]         = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_ButtonActive]          = ImVec4(0.26f, 0.66f, 0.23f, 1.00f);
    c[ImGuiCol_Header]                = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_HeaderHovered]         = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_HeaderActive]          = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_Separator]             = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
    c[ImGuiCol_SeparatorHovered]      = ImVec4(0.13f, 0.87f, 0.16f, 0.78f);
    c[ImGuiCol_SeparatorActive]       = ImVec4(0.25f, 0.75f, 0.10f, 1.00f);
    c[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.83f, 0.49f, 0.04f);
    c[ImGuiCol_ResizeGripHovered]     = ImVec4(0.28f, 0.71f, 0.25f, 0.78f);
    c[ImGuiCol_ResizeGripActive]      = ImVec4(0.28f, 0.71f, 0.25f, 1.00f);
    c[ImGuiCol_Tab]                   = ImVec4(0.26f, 0.67f, 0.23f, 0.95f);
    c[ImGuiCol_TabHovered]            = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_TabActive]             = ImVec4(0.24f, 0.60f, 0.00f, 1.00f);
    c[ImGuiCol_TabUnfocused]          = ImVec4(0.21f, 0.54f, 0.19f, 0.99f);
    c[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.24f, 0.60f, 0.21f, 1.00f);
    c[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    c[ImGuiCol_PlotLinesHovered]      = ImVec4(0.28f, 0.71f, 0.25f, 1.00f);
    c[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    c[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.28f, 0.71f, 0.25f, 1.00f);
    c[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.66f, 0.23f, 1.00f);
    c[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    c[ImGuiCol_NavHighlight]          = ImVec4(0.28f, 0.71f, 0.25f, 1.00f);
    c[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.16f, 0.16f, 0.16f, 0.73f);

    ImGuiStyle &s = ImGui::GetStyle();
    s.WindowRounding = 6.0;
    s.FrameRounding = 6.0;
    s.PopupRounding = 6.0;
    g_base_style = s;
}

void xemu_hud_init(SDL_Window* window, void* sdl_gl_context)
{
    xemu_monitor_init();
    g_vsync = g_config.display.window.vsync;

    InitCustomRendering();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = NULL;

    // Setup Platform/Renderer bindings
#ifdef CONFIG_SWITCH
    ImGui_ImplSDL2_InitForOpenGL(window, sdl_gl_context);
    ImGui_ImplOpenGL3_Init("#version 430");
    g_sdl_window = window;
#else
    ImGui_ImplSDL2_InitForOpenGL(window, sdl_gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");
    g_sdl_window = window;
#endif
    ImPlot::CreateContext();

#if defined(_WIN32)
    if (!g_config.general.show_welcome && g_config.general.updates.check) {
        update_window.CheckForUpdates();
    }
#endif
    g_last_scale = g_viewport_mgr.m_scale;
    InitializeStyle();
#ifdef CONFIG_SWITCH
    // Build 2: show the real main menu by default on Switch.
    // The welcome screen isn't useful without mouse/keyboard and blocks menu testing.
    first_boot_window.is_open = false;
    g_main_menu.SetNextViewIndexWithFocus(g_config.general.last_viewed_menu_index);
#else
    g_main_menu.SetNextViewIndex(g_config.general.last_viewed_menu_index);
    first_boot_window.is_open = g_config.general.show_welcome;
#endif
}

void xemu_hud_cleanup(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void xemu_hud_process_sdl_events(SDL_Event *event)
{
    // Ignore inputs that are consumed by rebinding
    if (g_main_menu.ConsumeRebindEvent(event)) {
        return;
    }

    ImGui_ImplSDL2_ProcessEvent(event);
}

#ifdef CONFIG_SWITCH
// Update ImGui gamepad state from Switch controller
extern "C" void xemu_hud_update_gamepad_state(uint64_t buttons_down, uint64_t buttons_held,
                                               float lstick_x, float lstick_y,
                                               float rstick_x, float rstick_y)
{
    ImGuiIO& io = ImGui::GetIO();

    // Switch button definitions from libnx
    const uint64_t HidNpadButton_A           = (1ULL << 0);
    const uint64_t HidNpadButton_B           = (1ULL << 1);
    const uint64_t HidNpadButton_X           = (1ULL << 2);
    const uint64_t HidNpadButton_Y           = (1ULL << 3);
    const uint64_t HidNpadButton_StickL      = (1ULL << 4);
    const uint64_t HidNpadButton_StickR      = (1ULL << 5);
    const uint64_t HidNpadButton_L           = (1ULL << 6);
    const uint64_t HidNpadButton_R           = (1ULL << 7);
    const uint64_t HidNpadButton_ZL          = (1ULL << 8);
    const uint64_t HidNpadButton_ZR          = (1ULL << 9);
    const uint64_t HidNpadButton_Plus        = (1ULL << 10);
    const uint64_t HidNpadButton_Minus       = (1ULL << 11);
    const uint64_t HidNpadButton_Left        = (1ULL << 12);
    const uint64_t HidNpadButton_Up          = (1ULL << 13);
    const uint64_t HidNpadButton_Right       = (1ULL << 14);
    const uint64_t HidNpadButton_Down        = (1ULL << 15);

    // Always feed current state every frame (ImGui expects continuous "held" state for repeats).
    auto set_button = [&](ImGuiKey key, uint64_t button_bit) {
        io.AddKeyEvent(key, (buttons_held & button_bit) != 0);
    };

    // Map face/shoulder/etc buttons (direct).
    set_button(ImGuiKey_GamepadFaceDown, HidNpadButton_A);
    set_button(ImGuiKey_GamepadFaceRight, HidNpadButton_B);
    set_button(ImGuiKey_GamepadFaceLeft, HidNpadButton_Y);
    set_button(ImGuiKey_GamepadFaceUp, HidNpadButton_X);
    set_button(ImGuiKey_GamepadL1, HidNpadButton_L);
    set_button(ImGuiKey_GamepadR1, HidNpadButton_R);
    set_button(ImGuiKey_GamepadL2, HidNpadButton_ZL);
    set_button(ImGuiKey_GamepadR2, HidNpadButton_ZR);
    set_button(ImGuiKey_GamepadStart, HidNpadButton_Plus);
    set_button(ImGuiKey_GamepadBack, HidNpadButton_Minus);
    set_button(ImGuiKey_GamepadL3, HidNpadButton_StickL);
    set_button(ImGuiKey_GamepadR3, HidNpadButton_StickR);

    // Map analog sticks (normalized -1..+1 from libnx) to ImGui's 0..1 analog values.
    const float deadzone = 0.15f;
    auto analog_mag = [&](float v) -> float {
        float av = fabsf(v);
        if (av <= deadzone) {
            return 0.0f;
        }
        float mag = (av - deadzone) / (1.0f - deadzone);
        if (mag < 0.0f) mag = 0.0f;
        if (mag > 1.0f) mag = 1.0f;
        return mag;
    };

    float lx = analog_mag(lstick_x);
    float ly = analog_mag(lstick_y);
    float rx = analog_mag(rstick_x);
    float ry = analog_mag(rstick_y);

    bool left = lstick_x < -deadzone;
    bool right = lstick_x > deadzone;
    bool up = lstick_y > deadzone;
    bool down = lstick_y < -deadzone;
    bool rleft = rstick_x < -deadzone;
    bool rright = rstick_x > deadzone;
    bool rup = rstick_y > deadzone;
    bool rdown = rstick_y < -deadzone;

    // When a direction isn't active, pass analog value = 0 to avoid canceling opposite directions.
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickLeft, left, left ? lx : 0.0f);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickRight, right, right ? lx : 0.0f);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickUp, up, up ? ly : 0.0f);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickDown, down, down ? ly : 0.0f);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickLeft, rleft, rleft ? rx : 0.0f);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickRight, rright, rright ? rx : 0.0f);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickUp, rup, rup ? ry : 0.0f);
    io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickDown, rdown, rdown ? ry : 0.0f);

    // ImGui's default nav movement uses D-pad keys for move requests, not LStick keys.
    // Mirror left stick directions into D-pad keys so analog can navigate the UI.
    const bool dpad_left_btn = (buttons_held & HidNpadButton_Left) != 0;
    const bool dpad_right_btn = (buttons_held & HidNpadButton_Right) != 0;
    const bool dpad_up_btn = (buttons_held & HidNpadButton_Up) != 0;
    const bool dpad_down_btn = (buttons_held & HidNpadButton_Down) != 0;

    const bool dpad_left = dpad_left_btn || left;
    const bool dpad_right = dpad_right_btn || right;
    const bool dpad_up = dpad_up_btn || up;
    const bool dpad_down = dpad_down_btn || down;

    io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadLeft, dpad_left, dpad_left_btn ? 1.0f : (left ? lx : 0.0f));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadRight, dpad_right, dpad_right_btn ? 1.0f : (right ? lx : 0.0f));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadUp, dpad_up, dpad_up_btn ? 1.0f : (up ? ly : 0.0f));
    io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadDown, dpad_down, dpad_down_btn ? 1.0f : (down ? ly : 0.0f));

    // Debug logging
    static int input_debug_count = 0;
    if (input_debug_count < 20) {
        if (buttons_down != 0) {
            fprintf(stderr, "BTN: down=0x%llx A=%d B=%d DPad=%d%d%d%d\n",
                    (unsigned long long)buttons_down,
                    (buttons_down & HidNpadButton_A) ? 1 : 0,
                    (buttons_down & HidNpadButton_B) ? 1 : 0,
                    (buttons_down & HidNpadButton_Up) ? 1 : 0,
                    (buttons_down & HidNpadButton_Down) ? 1 : 0,
                    (buttons_down & HidNpadButton_Left) ? 1 : 0,
                    (buttons_down & HidNpadButton_Right) ? 1 : 0);
            input_debug_count++;
        }
        if (lx > 0.0f || ly > 0.0f) {
            fprintf(stderr, "LSTICK: x=%.2f y=%.2f\n", lstick_x, lstick_y);
            input_debug_count++;
        }
    }
}
#endif

void xemu_hud_should_capture_kbd_mouse(int *kbd, int *mouse)
{
    ImGuiIO& io = ImGui::GetIO();
    if (kbd) *kbd = io.WantCaptureKeyboard;
    if (mouse) *mouse = io.WantCaptureMouse;
}

void xemu_hud_set_framebuffer_texture(GLuint tex, bool flip)
{
    g_tex = tex;
    g_flip_req = flip;
}

void xemu_hud_render(void)
{
#ifdef CONFIG_SWITCH
    static int render_debug_count = 0;
    #define RENDER_DBG(msg) do { if (render_debug_count < 3) { fprintf(stderr, "render: %s\n", msg); fflush(stderr); } } while(0)
#else
    #define RENDER_DBG(msg) ((void)0)
#endif

    RENDER_DBG("start");

#ifdef CONFIG_SWITCH
    // Clear the screen with a visible color on Switch
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);  // Dark blue-gray
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int gl_w = 0;
    int gl_h = 0;
    SDL_GL_GetDrawableSize(g_sdl_window, &gl_w, &gl_h);
    glViewport(0, 0, gl_w, gl_h);
#endif

    ImGuiIO& io = ImGui::GetIO();
    uint32_t now = SDL_GetTicks();

    RENDER_DBG("viewport update");
    g_viewport_mgr.Update();
    RENDER_DBG("font update");
    g_font_mgr.Update();
    if (g_last_scale != g_viewport_mgr.m_scale) {
        ImGuiStyle &style = ImGui::GetStyle();
        style = g_base_style;
        style.ScaleAllSizes(g_viewport_mgr.m_scale);
        g_last_scale = g_viewport_mgr.m_scale;
    }

    RENDER_DBG("framebuffer check");
    if (!first_boot_window.is_open) {
        int ww, wh;
        SDL_GL_GetDrawableSize(g_sdl_window, &ww, &wh);
        RENDER_DBG("RenderFramebuffer");
        RenderFramebuffer(g_tex, ww, wh, g_flip_req);
    }

    RENDER_DBG("ImGui_ImplOpenGL3_NewFrame");
    ImGui_ImplOpenGL3_NewFrame();
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplSDL2_NewFrame();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    RENDER_DBG("input update");
    g_input_mgr.Update();

    RENDER_DBG("ImGui::NewFrame");
    ImGui::NewFrame();
    RENDER_DBG("ProcessKeyboardShortcuts");
    ProcessKeyboardShortcuts();

#ifdef CONFIG_SWITCH
    render_debug_count++;

    // Allow reopening the main menu after it is closed.
    // Map: Minus (ImGuiKey_GamepadBack) -> open menu when no scene is active.
    if (!g_scene_mgr.IsDisplayingScene() &&
        ImGui::IsKeyPressed(ImGuiKey_GamepadBack)) {
        g_main_menu.SetNextViewIndexWithFocus(g_config.general.last_viewed_menu_index);
    }
#endif

#if defined(CONFIG_RENDERDOC)
    if (g_capture_renderdoc_frame) {
        nv2a_dbg_renderdoc_capture_frames(1, false);
        g_capture_renderdoc_frame = false;
    }
#endif

    if (g_config.display.ui.show_menubar && !first_boot_window.is_open) {
        // Auto-hide main menu after 5s of inactivity
        static uint32_t last_check = 0;
        float alpha = 1.0;
        const uint32_t timeout = 5000;
        const float fade_duration = 1000.0;
        bool menu_wakeup = g_input_mgr.MouseMoved();
        if (menu_wakeup) {
            last_check = now;
        }
        if ((now-last_check) > timeout) {
            if (g_config.display.ui.use_animations) {
                float t = fmin((float)((now-last_check)-timeout)/fade_duration, 1);
                alpha = 1-t;
                if (t >= 1) {
                    alpha = 0;
                }
            } else {
                alpha = 0;
            }
        }
        if (alpha > 0.0) {
            ImVec4 tc = ImGui::GetStyle().Colors[ImGuiCol_Text];
            tc.w = alpha;
            ImGui::PushStyleColor(ImGuiCol_Text, tc);
            ImGui::SetNextWindowBgAlpha(alpha);
            ShowMainMenu();
            ImGui::PopStyleColor();
        } else {
            g_main_menu_height = 0;
        }
    }

    static uint32_t last_mouse_move = 0;
    if (g_input_mgr.MouseMoved()) {
        last_mouse_move = now;
    }

    // FIXME: Handle time wrap around
    if (g_config.display.ui.hide_cursor && (now - last_mouse_move) > 3000) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    }

    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) &&
        !g_scene_mgr.IsDisplayingScene()) {

        // If the guide button is pressed, wake the ui
        bool menu_button = false;
        uint32_t buttons = g_input_mgr.CombinedButtons();
        if (buttons & CONTROLLER_BUTTON_GUIDE) {
            menu_button = true;
        }

        // Allow controllers without a guide button to also work
        if ((buttons & CONTROLLER_BUTTON_BACK) &&
            (buttons & CONTROLLER_BUTTON_START)) {
            menu_button = true;
        }

        if (ImGui::IsKeyPressed(ImGuiKey_F1)) {
            g_scene_mgr.PushScene(g_main_menu);
        } else if (ImGui::IsKeyPressed(ImGuiKey_F2)) {
            g_scene_mgr.PushScene(g_popup_menu);
        } else if (menu_button ||
                   (ImGui::IsMouseClicked(ImGuiMouseButton_Right) &&
                    !ImGui::IsAnyItemFocused() && !ImGui::IsAnyItemHovered())) {
            g_scene_mgr.PushScene(g_popup_menu);
        } else if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            xemu_toggle_fullscreen();
        }

        bool mod_key_down = ImGui::IsKeyDown(ImGuiKey_ModShift);
        for (int f_key = 0; f_key < 4; ++f_key) {
            if (ImGui::IsKeyPressed((enum ImGuiKey)(ImGuiKey_F5 + f_key))) {
                ActionActivateBoundSnapshot(f_key, mod_key_down);
                break;
            }
        }
    }

    first_boot_window.Draw();
    monitor_window.Draw();
    apu_window.Draw();
    video_window.Draw();
    compatibility_reporter_window.Draw();
#if defined(_WIN32)
    update_window.Draw();
#endif
    g_scene_mgr.Draw();
    if (!first_boot_window.is_open) notification_manager.Draw();
    g_snapshot_mgr.Draw();

    // static bool show_demo = true;
    // if (show_demo) ImGui::ShowDemoWindow(&show_demo);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (g_vsync != g_config.display.window.vsync) {
        g_vsync = g_config.display.window.vsync;
        SDL_GL_SetSwapInterval(g_vsync ? 1 : 0);
    }

    if (g_screenshot_pending) {
        SaveScreenshot(g_tex, g_flip_req);
        g_screenshot_pending = false;
    }
}
