/*
 * C++ class stubs for Nintendo Switch
 *
 * Provides stub implementations for C++ classes that are referenced
 * by the UI code but not fully available on Switch.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <SDL.h>
#include "ui/xemu-input.h"
#include "ui/xemu-controllers.h"

/*
 * ControllerKeyboardRebindingMap stub implementation
 */
RebindEventResult ControllerKeyboardRebindingMap::ConsumeRebindEvent(SDL_Event *event)
{
    (void)event;
    return RebindEventResult::Ignore;
}

/*
 * ControllerGamepadRebindingMap stub implementations
 */
RebindEventResult ControllerGamepadRebindingMap::ConsumeRebindEvent(SDL_Event *event)
{
    (void)event;
    return RebindEventResult::Ignore;
}

RebindEventResult ControllerGamepadRebindingMap::HandleButtonEvent(SDL_ControllerButtonEvent *event)
{
    (void)event;
    return RebindEventResult::Ignore;
}

RebindEventResult ControllerGamepadRebindingMap::HandleAxisEvent(SDL_ControllerAxisEvent *event)
{
    (void)event;
    return RebindEventResult::Ignore;
}

/*
 * Debug window class stubs
 * These need to match the declarations in the UI code
 */
class DebugApuWindow {
public:
    bool m_visible;
    DebugApuWindow() : m_visible(false) {}
    void Draw();
};

void DebugApuWindow::Draw()
{
    /* Stub - do nothing */
}

class DebugVideoWindow {
public:
    bool m_visible;
    DebugVideoWindow() : m_visible(false) {}
    void Draw();
};

void DebugVideoWindow::Draw()
{
    /* Stub - do nothing */
}

class CompatibilityReporter {
public:
    bool m_visible;
    CompatibilityReporter() : m_visible(false) {}
    void Draw();
};

void CompatibilityReporter::Draw()
{
    /* Stub - do nothing */
}
