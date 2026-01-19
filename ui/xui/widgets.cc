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
#include "widgets.hh"
#include "misc.hh"
#include "font-manager.hh"
#include "viewport-manager.hh"
#include "ui/xemu-os-utils.h"
#include "gl-helpers.hh"

#ifdef CONFIG_SWITCH
#include "xemu-settings.h"
#include <dirent.h>
#include <sys/stat.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#endif

#ifdef CONFIG_SWITCH
namespace {

struct SwitchFileDialogEntry {
    std::string name;
    bool is_dir = false;
};

struct SwitchFileDialogState {
    bool active = false;
    bool popup_opened = false;
    bool dir_mode = false;
    const char *filters = nullptr;
    const char **target = nullptr;

    std::string cwd;
    std::vector<std::string> allowed_exts;
    std::vector<SwitchFileDialogEntry> entries;
    int selected = 0;

    bool has_result = false;
    std::string result;

    int last_drawn_frame = -1;
};

static SwitchFileDialogState g_switch_file_dialog;

static std::string ToLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return (char)std::tolower(c); });
    return s;
}

static bool PathIsDir(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return false;
    }
    return S_ISDIR(st.st_mode);
}

static std::string NormalizeDir(std::string path)
{
    if (path.empty()) {
        return "sdmc:/";
    }
    if (path.back() != '/') {
        path.push_back('/');
    }
    return path;
}

static std::string ParentDir(std::string path)
{
    if (path.empty()) {
        return path;
    }
    while (!path.empty() && path.back() == '/') {
        path.pop_back();
    }
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return "";
    }
    return path.substr(0, pos + 1);
}

static std::string JoinPath(const std::string &dir, const std::string &name)
{
    if (dir.empty()) {
        return name;
    }
    if (dir.back() == '/') {
        return dir + name;
    }
    return dir + "/" + name;
}

static std::vector<std::string> ParseAllowedExtensions(const char *filters)
{
    std::vector<std::string> exts;
    if (!filters) {
        return exts;
    }

    const char *p = filters;
    while (*p) {
        // Skip name
        p += strlen(p) + 1;
        if (!*p) {
            break;
        }
        // Pattern(s)
        std::string patterns = p;
        p += strlen(p) + 1;

        size_t start = 0;
        while (start < patterns.size()) {
            size_t end = patterns.find(';', start);
            if (end == std::string::npos) {
                end = patterns.size();
            }
            std::string pat = patterns.substr(start, end - start);
            pat = ToLower(pat);
            if (pat.rfind("*.", 0) == 0 && pat.size() > 2) {
                exts.push_back(pat.substr(2));
            }
            start = end + 1;
        }
    }

    std::sort(exts.begin(), exts.end());
    exts.erase(std::unique(exts.begin(), exts.end()), exts.end());
    return exts;
}

static bool PassesFilter(const std::string &name,
                         const std::vector<std::string> &allowed_exts)
{
    if (allowed_exts.empty()) {
        return true;
    }
    size_t dot = name.find_last_of('.');
    if (dot == std::string::npos || dot + 1 >= name.size()) {
        return false;
    }
    std::string ext = ToLower(name.substr(dot + 1));
    return std::find(allowed_exts.begin(), allowed_exts.end(), ext) !=
           allowed_exts.end();
}

static void RefreshDirectoryListing()
{
    auto &st = g_switch_file_dialog;
    st.entries.clear();
    st.selected = 0;

    DIR *d = opendir(st.cwd.c_str());
    if (!d) {
        return;
    }

    // Add parent shortcut if not at root.
    if (st.cwd != "sdmc:/" && st.cwd != "/") {
        st.entries.push_back({ "..", true });
    }

    std::vector<SwitchFileDialogEntry> dirs;
    std::vector<SwitchFileDialogEntry> files;

    struct dirent *ent;
    while ((ent = readdir(d)) != nullptr) {
        const char *n = ent->d_name;
        if (!n || n[0] == '\0') {
            continue;
        }
        if (strcmp(n, ".") == 0 || strcmp(n, "..") == 0) {
            continue;
        }

        std::string name = n;
        std::string full = JoinPath(st.cwd, name);

        bool is_dir = false;
#ifdef DT_DIR
        if (ent->d_type == DT_DIR) {
            is_dir = true;
        } else if (ent->d_type == DT_UNKNOWN) {
            is_dir = PathIsDir(full);
        }
#else
        is_dir = PathIsDir(full);
#endif

        if (is_dir) {
            dirs.push_back({ name, true });
        } else if (!st.dir_mode && PassesFilter(name, st.allowed_exts)) {
            files.push_back({ name, false });
        } else if (st.dir_mode) {
            // Hide files when selecting directories.
        }
    }

    closedir(d);

    auto by_name = [](const SwitchFileDialogEntry &a,
                      const SwitchFileDialogEntry &b) {
        return ToLower(a.name) < ToLower(b.name);
    };
    std::sort(dirs.begin(), dirs.end(), by_name);
    std::sort(files.begin(), files.end(), by_name);

    st.entries.insert(st.entries.end(), dirs.begin(), dirs.end());
    st.entries.insert(st.entries.end(), files.begin(), files.end());
}

static void SwitchFileDialogOpen(const char **target, bool dir_mode,
                                 const char *filters, const char *default_path)
{
    auto &st = g_switch_file_dialog;
    st.active = true;
    st.popup_opened = false;
    st.dir_mode = dir_mode;
    st.filters = filters;
    st.target = target;
    st.allowed_exts = ParseAllowedExtensions(filters);
    st.has_result = false;
    st.result.clear();

    std::string start = default_path ? default_path : "";
    if (start.empty() && target && *target) {
        start = *target;
    }
    if (start.empty()) {
        start = "sdmc:/switch/xemu/";
    }

    // If start is a file, use its parent directory.
    if (!PathIsDir(start)) {
        start = ParentDir(start);
    }
    if (start.empty()) {
        start = "sdmc:/";
    }
    st.cwd = NormalizeDir(start);

    RefreshDirectoryListing();
}

static bool SwitchFileDialogConsumeResult(const char **target)
{
    auto &st = g_switch_file_dialog;
    if (!st.has_result || st.target != target) {
        return false;
    }
    return true;
}

static void SwitchFileDialogClose()
{
    auto &st = g_switch_file_dialog;
    st.active = false;
    st.popup_opened = false;
    st.filters = nullptr;
    st.target = nullptr;
    st.allowed_exts.clear();
    st.entries.clear();
    st.selected = 0;
    st.has_result = false;
    st.result.clear();
}

static void SwitchFileDialogDrawOnce()
{
    auto &st = g_switch_file_dialog;
    if (!st.active) {
        return;
    }

    const int frame = ImGui::GetFrameCount();
    if (st.last_drawn_frame == frame) {
        return;
    }
    st.last_drawn_frame = frame;

    const char *popup_name = "Select Path###switch_file_dialog";
    if (!st.popup_opened) {
        ImGui::OpenPopup(popup_name);
        st.popup_opened = true;
    }

    ImGui::SetNextWindowSize(ImVec2(900 * g_viewport_mgr.m_scale,
                                    600 * g_viewport_mgr.m_scale),
                             ImGuiCond_Appearing);

    bool open = true;
    if (ImGui::BeginPopupModal(popup_name, &open,
                               ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::TextWrapped("%s", st.cwd.c_str());
        ImGui::Separator();

        ImGui::BeginChild("###file_list", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2),
                          true, ImGuiWindowFlags_NavFlattened);

        if (st.entries.empty()) {
            ImGui::Text("(Empty)");
        } else {
            for (int i = 0; i < (int)st.entries.size(); i++) {
                const auto &e = st.entries[i];
                std::string label = e.is_dir ? (std::string("[DIR] ") + e.name)
                                             : e.name;
                bool selected = (i == st.selected);
                if (ImGui::Selectable(label.c_str(), selected)) {
                    st.selected = i;
                }
                if (ImGui::IsItemActivated()) {
                    if (e.is_dir) {
                        if (e.name == "..") {
                            std::string parent = ParentDir(st.cwd);
                            if (!parent.empty()) {
                                st.cwd = NormalizeDir(parent);
                            }
                        } else {
                            st.cwd = NormalizeDir(JoinPath(st.cwd, e.name));
                        }
                        RefreshDirectoryListing();
                    } else {
                        st.result = JoinPath(st.cwd, e.name);
                        st.has_result = true;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
        }

        ImGui::EndChild();

        // Controller-friendly shortcuts
        if (ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight)) {
            // B: go up if possible, otherwise cancel.
            if (st.cwd != "sdmc:/" && st.cwd != "/") {
                std::string parent = ParentDir(st.cwd);
                if (!parent.empty()) {
                    st.cwd = NormalizeDir(parent);
                    RefreshDirectoryListing();
                }
            } else {
                st.has_result = false;
                st.result.clear();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::Separator();
        if (ImGui::Button("Cancel")) {
            st.has_result = false;
            st.result.clear();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        bool can_select = st.dir_mode;
        if (!st.dir_mode && st.selected >= 0 && st.selected < (int)st.entries.size()) {
            can_select = !st.entries[st.selected].is_dir;
        }
        if (!can_select) {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button(st.dir_mode ? "Select Folder" : "Select File")) {
            if (st.dir_mode) {
                st.result = st.cwd;
                st.has_result = true;
                ImGui::CloseCurrentPopup();
            } else if (st.selected >= 0 && st.selected < (int)st.entries.size()) {
                const auto &e = st.entries[st.selected];
                if (!e.is_dir) {
                    st.result = JoinPath(st.cwd, e.name);
                    st.has_result = true;
                    ImGui::CloseCurrentPopup();
                }
            }
        }
        if (!can_select) {
            ImGui::EndDisabled();
        }

        ImGui::EndPopup();
    }

    if (!open) {
        SwitchFileDialogClose();
    } else if (st.popup_opened && !ImGui::IsPopupOpen(popup_name)) {
        // Popup was closed via CloseCurrentPopup().
        if (!st.has_result) {
            SwitchFileDialogClose();
        }
    }
}

} // namespace
#endif

void Separator()
{
    // XXX: IDK. Maybe there's a better way to draw a separator ( ImGui::Separator() ) that cuts through window
    //      padding... Just grab the draw list and draw the line with outer clip rect

    float thickness = 1 * g_viewport_mgr.m_scale;

    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImRect window_rect = window->Rect();
    ImVec2 size = ImVec2(window_rect.GetWidth(), thickness);

    ImVec2 p0(window_rect.Min.x, ImGui::GetCursorScreenPos().y);
    ImVec2 p1(p0.x + size.x, p0.y);
    ImGui::PushClipRect(window_rect.Min, window_rect.Max, false);
    draw_list->AddLine(p0, p1, ImGui::GetColorU32(ImGuiCol_Separator), thickness);
    ImGui::PopClipRect();
    ImGui::Dummy(size);
}

void SectionTitle(const char *title)
{
    ImGui::Spacing();
    ImGui::PushFont(g_font_mgr.m_menu_font_medium);
    ImGui::Text("%s", title);
    ImGui::PopFont();
    Separator();
}

float GetWidgetTitleDescriptionHeight(const char *title,
                                      const char *description)
{
    ImGui::PushFont(g_font_mgr.m_menu_font_medium);
    float h = ImGui::GetFrameHeight();
    ImGui::PopFont();

    if (description) {
        ImGuiStyle &style = ImGui::GetStyle();
        h += style.ItemInnerSpacing.y;
        ImGui::PushFont(g_font_mgr.m_default_font);
        h += ImGui::GetTextLineHeight();
        ImGui::PopFont();
    }

    return h;
}

void WidgetTitleDescription(const char *title, const char *description,
                            ImVec2 pos)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImGuiStyle &style = ImGui::GetStyle();

    ImVec2 text_pos = pos;
    text_pos.x += style.FramePadding.x;
    text_pos.y += style.FramePadding.y;

    ImGui::PushFont(g_font_mgr.m_menu_font_medium);
    float title_height = ImGui::GetTextLineHeight();
    draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), title);
    ImGui::PopFont();

    if (description) {
        text_pos.y += title_height + style.ItemInnerSpacing.y;

        ImGui::PushFont(g_font_mgr.m_default_font);
        draw_list->AddText(text_pos, ImGui::GetColorU32(ImVec4(0.94f, 0.94f, 0.94f, 0.70f)), description);
        ImGui::PopFont();
    }
}

void WidgetTitleDescriptionItem(const char *str_id, const char *description)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 size(ImGui::GetColumnWidth(),
                GetWidgetTitleDescriptionHeight(str_id, description));
    WidgetTitleDescription(str_id, description, p);

    // XXX: Internal API
    ImRect bb(p, ImVec2(p.x + size.x, p.y + size.y));
    ImGui::ItemSize(size, 0.0f);
    ImGui::ItemAdd(bb, 0);
}

float GetSliderRadius(ImVec2 size)
{
    return size.y * 0.5;
}

float GetSliderTrackXOffset(ImVec2 size)
{
    return GetSliderRadius(size);
}

float GetSliderTrackWidth(ImVec2 size)
{
    return size.x - GetSliderRadius(size) * 2;
}

float GetSliderValueForMousePos(ImVec2 mouse, ImVec2 pos, ImVec2 size)
{
    return (mouse.x - pos.x - GetSliderTrackXOffset(size)) /
           GetSliderTrackWidth(size);
}

void DrawSlider(float v, bool hovered, ImVec2 pos, ImVec2 size)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    float radius = GetSliderRadius(size);
    float rounding = size.y * 0.25;
    float slot_half_height = size.y * 0.125;
    const bool circular_grab = false;

    ImU32 bg = hovered ? ImGui::GetColorU32(ImGuiCol_FrameBgActive)
                       : ImGui::GetColorU32(ImGuiCol_CheckMark);

    ImVec2 pmid(pos.x + radius + v*(size.x - radius*2), pos.y + size.y / 2);
    ImVec2 smin(pos.x + rounding, pmid.y - slot_half_height);
    ImVec2 smax(pmid.x, pmid.y + slot_half_height);
    draw_list->AddRectFilled(smin, smax, bg, rounding);

    bg = hovered ? ImGui::GetColorU32(ImGuiCol_FrameBgHovered)
                 : ImGui::GetColorU32(ImGuiCol_FrameBg);

    smin.x = pmid.x;
    smax.x = pos.x + size.x - rounding;
    draw_list->AddRectFilled(smin, smax, bg, rounding);

    if (circular_grab) {
       draw_list->AddCircleFilled(pmid, radius * 0.8, ImGui::GetColorU32(ImGuiCol_SliderGrab));
    } else {
        ImVec2 offs(radius*0.8, radius*0.8);
        draw_list->AddRectFilled(pmid - offs, pmid + offs, ImGui::GetColorU32(ImGuiCol_SliderGrab), rounding);
    }
}

void DrawToggle(bool enabled, bool hovered, ImVec2 pos, ImVec2 size)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    float radius = size.y * 0.5;
    float rounding = size.y * 0.25;
    float slot_half_height = size.y * 0.5;
    const bool circular_grab = false;

    ImU32 bg = hovered ? ImGui::GetColorU32(enabled ? ImGuiCol_FrameBgActive : ImGuiCol_FrameBgHovered)
                       : ImGui::GetColorU32(enabled ? ImGuiCol_CheckMark : ImGuiCol_FrameBg);

    ImVec2 pmid(pos.x + radius + (int)enabled * (size.x - radius * 2), pos.y + size.y / 2);
    ImVec2 smin(pos.x, pmid.y - slot_half_height);
    ImVec2 smax(pos.x + size.x, pmid.y + slot_half_height);
    draw_list->AddRectFilled(smin, smax, bg, rounding);

    if (circular_grab) {
        draw_list->AddCircleFilled(pmid, radius * 0.8, ImGui::GetColorU32(ImGuiCol_SliderGrab));
    } else {
        ImVec2 offs(radius*0.8, radius*0.8);
        draw_list->AddRectFilled(pmid - offs, pmid + offs, ImGui::GetColorU32(ImGuiCol_SliderGrab), rounding);
    }
}

bool Toggle(const char *str_id, bool *v, const char *description)
{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);

    ImGuiStyle &style = ImGui::GetStyle();

    ImGui::PushFont(g_font_mgr.m_menu_font_medium);
    float title_height = ImGui::GetTextLineHeight();
    ImGui::PopFont();

    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 bb(ImGui::GetColumnWidth(),
              GetWidgetTitleDescriptionHeight(str_id, description));
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
    ImGui::PushID(str_id);
    bool status = ImGui::Button("###toggle_button", bb);
    if (status) {
        *v = !*v;
    }
    ImGui::PopID();
    ImGui::PopStyleVar();
    const ImVec2 p_min = ImGui::GetItemRectMin();
    const ImVec2 p_max = ImGui::GetItemRectMax();

    WidgetTitleDescription(str_id, description, p);

    float toggle_height = title_height * 0.9;
    ImVec2 toggle_size(toggle_height * 1.75, toggle_height);
    ImVec2 toggle_pos(p_max.x - toggle_size.x - style.FramePadding.x,
                      p_min.y + (title_height - toggle_size.y)/2 + style.FramePadding.y);
    DrawToggle(*v, ImGui::IsItemHovered(), toggle_pos, toggle_size);

    ImGui::PopStyleColor();

    return status;
}

void Slider(const char *str_id, float *v, const char *description)
{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);

    ImGuiStyle &style = ImGui::GetStyle();
    ImGuiWindow *window = ImGui::GetCurrentWindow();

    ImGui::PushID(str_id);

    ImGui::PushFont(g_font_mgr.m_menu_font_medium);
    float title_height = ImGui::GetTextLineHeight();
    ImGui::PopFont();

    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 size(ImGui::GetColumnWidth(),
                GetWidgetTitleDescriptionHeight(str_id, description));
    WidgetTitleDescription(str_id, description, p);

    // XXX: Internal API
    ImVec2 wpos = ImGui::GetCursorPos();
    ImRect bb(p, ImVec2(p.x + size.x, p.y + size.y));
    ImGui::ItemSize(size, 0.0f);
    ImGui::ItemAdd(bb, 0);
    ImGui::SetItemAllowOverlap();
    ImGui::SameLine(0, 0);

    ImVec2 slider_size(size.x * 0.4, title_height * 0.9);
    ImVec2 slider_pos(bb.Max.x - slider_size.x - style.FramePadding.x,
                      p.y + (title_height - slider_size.y)/2 + style.FramePadding.y);

    ImGui::SetCursorPos(ImVec2(wpos.x + size.x - slider_size.x - style.FramePadding.x,
                               wpos.y));

    ImGui::InvisibleButton("###slider", slider_size, 0);

    // Allow controller/keyboard adjustment when the item is nav-focused as well.
    if (ImGui::IsItemHovered() || ImGui::IsItemFocused()) {
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) ||
            ImGui::IsKeyPressed(ImGuiKey_GamepadDpadLeft) ||
            ImGui::IsKeyPressed(ImGuiKey_GamepadLStickLeft) ||
            ImGui::IsKeyPressed(ImGuiKey_GamepadRStickLeft)) {
                *v -= 0.05;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow) ||
            ImGui::IsKeyPressed(ImGuiKey_GamepadDpadRight) ||
            ImGui::IsKeyPressed(ImGuiKey_GamepadLStickRight) ||
            ImGui::IsKeyPressed(ImGuiKey_GamepadRStickRight)) {
                *v += 0.05;
        }

        if (
            ImGui::IsKeyDown(ImGuiKey_LeftArrow) ||
            ImGui::IsKeyDown(ImGuiKey_GamepadDpadLeft) ||
            ImGui::IsKeyDown(ImGuiKey_GamepadLStickLeft) ||
            ImGui::IsKeyDown(ImGuiKey_GamepadRStickLeft) ||
            ImGui::IsKeyDown(ImGuiKey_RightArrow) ||
            ImGui::IsKeyDown(ImGuiKey_GamepadDpadRight) ||
            ImGui::IsKeyDown(ImGuiKey_GamepadLStickRight) ||
            ImGui::IsKeyDown(ImGuiKey_GamepadRStickRight)
            ) {
            ImGui::NavMoveRequestCancel();
        }
    }

    if (ImGui::IsItemActive()) {
        ImVec2 mouse = ImGui::GetMousePos();
        *v = GetSliderValueForMousePos(mouse, slider_pos, slider_size);
    }
    *v = fmax(0, fmin(*v, 1));
    DrawSlider(*v, ImGui::IsItemHovered() || ImGui::IsItemActive(), slider_pos,
               slider_size);

    ImVec2 slider_max = ImVec2(slider_pos.x + slider_size.x, slider_pos.y + slider_size.y);
    ImGui::RenderNavHighlight(ImRect(slider_pos, slider_max), window->GetID("###slider"));

    ImGui::PopStyleColor();
    ImGui::PopID();
}

bool FilePicker(const char *str_id, const char **buf, const char *filters,
                bool dir)
{
    bool changed = false;

#ifdef CONFIG_SWITCH
    // Consume selection from the Switch file dialog (if it targeted this picker).
    if (SwitchFileDialogConsumeResult(buf)) {
        auto &st = g_switch_file_dialog;
        if (*buf) {
            free((void *)*buf);
        }
        *buf = strdup(st.result.c_str());
        st.has_result = false;
        st.result.clear();
        SwitchFileDialogClose();
        changed = true;
    }
#endif

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 cursor = ImGui::GetCursorPos();
    const char *desc = strlen(*buf) ? *buf : "(None Selected)";
    ImVec2 bb(ImGui::GetColumnWidth(),
              GetWidgetTitleDescriptionHeight(str_id, desc));
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
    ImGui::PushID(str_id);
    bool status =
        ImGui::ButtonEx("###file_button", bb, ImGuiButtonFlags_AllowOverlap);
    ImGui::SetItemAllowOverlap();
    if (status) {
        int flags = NOC_FILE_DIALOG_OPEN;
        if (dir) flags |= NOC_FILE_DIALOG_DIR;
#ifdef CONFIG_SWITCH
        SwitchFileDialogOpen(buf, dir, filters, *buf);
#else
        const char *new_path =
            PausedFileOpen(flags, filters, *buf, NULL);
        if (new_path) {
            free((void*)*buf);
            *buf = strdup(new_path);
            desc = *buf;
            changed = true;
        }
#endif
    }
    ImGui::PopID();
    ImGui::PopStyleVar();

    WidgetTitleDescription(str_id, desc, p);

    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImVec2 p1 = ImGui::GetItemRectMax();

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImGui::PushFont(g_font_mgr.m_menu_font);
    const char *icon = dir ? ICON_FA_FOLDER : ICON_FA_FILE;
    ImVec2 ts_icon = ImGui::CalcTextSize(icon);
    ImVec2 icon_pos = ImVec2(p1.x - style.FramePadding.x - ts_icon.x,
                              p0.y + (p1.y - p0.y - ts_icon.y) / 2);
    draw_list->AddText(icon_pos, ImGui::GetColorU32(ImGuiCol_Text), icon);

    ImVec2 ts_clear_icon = ImGui::CalcTextSize(ICON_FA_XMARK);
    ts_clear_icon.x += 2 * style.FramePadding.x;
    ImVec2 clear_icon_pos = ImVec2(cursor.x + bb.x - ts_icon.x - ts_clear_icon.x, cursor.y);

    auto prev_pos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(clear_icon_pos);

    char *clear_button_id = g_strdup_printf("%s_clear", str_id);
    ImGui::PushID(clear_button_id);

    bool clear = ImGui::Button(ICON_FA_XMARK, ImVec2(ts_clear_icon.x, bb.y));
    if (clear) {
        free((void*)*buf);
        *buf = strdup("");
        changed = true;
    }

    ImGui::PopID();
    g_free(clear_button_id);

    ImGui::SetCursorPos(prev_pos);

    ImGui::PopFont();

    ImGui::PopStyleColor();

#ifdef CONFIG_SWITCH
    SwitchFileDialogDrawOnce();
    if (changed) {
        // Persist immediately on Switch (app exit isn't always graceful).
        xemu_settings_save();
    }
#endif

    return changed;
}

void DrawComboChevron()
{
    ImGui::PushFont(g_font_mgr.m_menu_font);
    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImVec2 p1 = ImGui::GetItemRectMax();
    const char *icon = ICON_FA_CHEVRON_DOWN;
    ImVec2 ts_icon = ImGui::CalcTextSize(icon);
    ImGuiStyle &style = ImGui::GetStyle();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddText(ImVec2(p1.x - style.FramePadding.x - ts_icon.x,
                              p0.y + (p1.y - p0.y - ts_icon.y) / 2),
                       ImGui::GetColorU32(ImGuiCol_Text), icon);
    ImGui::PopFont();
}

void PrepareComboTitleDescription(const char *label, const char *description,
                                  float combo_size_ratio)
{
    float width = ImGui::GetColumnWidth();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size(width, GetWidgetTitleDescriptionHeight(label, description));
    WidgetTitleDescription(label, description, pos);

    ImVec2 wpos = ImGui::GetCursorPos();
    ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, 0.0f);
    ImGui::ItemAdd(bb, 0);
    ImGui::SetItemAllowOverlap();
    ImGui::SameLine(0, 0);
    float combo_width = width * combo_size_ratio;
    ImGui::SetCursorPos(ImVec2(wpos.x + width - combo_width, wpos.y));
}

bool ChevronCombo(const char *label, int *current_item,
                  bool (*items_getter)(void *, int, const char **), void *data,
                  int items_count, const char *description)
{
    bool value_changed = false;
    float combo_width = ImGui::GetColumnWidth();
    if (*label != '#') {
        float combo_size_ratio = 0.4;
        PrepareComboTitleDescription(label, description, combo_size_ratio);
        combo_width *= combo_size_ratio;
    }

    ImGuiContext& g = *GImGui;
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(1, 0));

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_value);

    ImGui::SetNextItemWidth(combo_width);
    ImGui::PushFont(g_font_mgr.m_menu_font_small);
    ImGui::PushID(label);
    if (ImGui::BeginCombo("###chevron_combo", preview_value, ImGuiComboFlags_NoArrowButton)) {
        // Display items
        // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
        for (int i = 0; i < items_count; i++)
        {
            ImGui::PushID(i);
            const bool item_selected = (i == *current_item);
            const char* item_text;
            if (!items_getter(data, i, &item_text))
                item_text = "*Unknown item*";
            if (ImGui::Selectable(item_text, item_selected))
            {
                value_changed = true;
                *current_item = i;
            }
            if (item_selected)
                ImGui::SetItemDefaultFocus();
            ImGui::PopID();
        }

        ImGui::EndCombo();

        if (value_changed)
            ImGui::MarkItemEdited(g.LastItemData.ID);
    }
    ImGui::PopID();
    ImGui::PopFont();
    DrawComboChevron();
    ImGui::PopStyleVar();
    return value_changed;
}

// Getter for the old Combo() API: "item1\0item2\0item3\0"
static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
    // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    if (!*p)
        return false;
    if (out_text)
        *out_text = p;
    return true;
}

// Combo box helper allowing to pass all items in a single string literal holding multiple zero-terminated items "item1\0item2\0"
bool ChevronCombo(const char* label, int* current_item, const char* items_separated_by_zeros, const char *description)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = ChevronCombo(
        label, current_item, Items_SingleStringGetter,
        (void *)items_separated_by_zeros, items_count, description);
    return value_changed;
}

void Hyperlink(const char *text, const char *url)
{
    ImColor col;
    ImGui::Text("%s", text);
    if (ImGui::IsItemHovered()) {
        col = IM_COL32_WHITE;
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    } else {
        col = ImColor(127, 127, 127, 255);
    }

    ImVec2 max = ImGui::GetItemRectMax();
    ImVec2 min = ImGui::GetItemRectMin();
    min.x -= 1 * g_viewport_mgr.m_scale;
    min.y = max.y;
    max.x -= 1 * g_viewport_mgr.m_scale;
    ImGui::GetWindowDrawList()->AddLine(min, max, col, 1.0 * g_viewport_mgr.m_scale);

    if (ImGui::IsItemClicked()) {
        SDL_OpenURL(url);
    }
}

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void Logo()
{
    ImGui::SetCursorPosY(ImGui::GetCursorPosY()-25*g_viewport_mgr.m_scale);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth()-256*g_viewport_mgr.m_scale)/2);

#ifdef CONFIG_SWITCH
    // Switch port: rendering the animated logo into an FBO every frame is
    // extremely expensive. Throttle redraws to keep the UI responsive.
    static uint32_t time_start = 0;
    static uint32_t last_render_ms = 0;
    static uint32_t last_logo_time = 0;

    const uint32_t now = SDL_GetTicks();
    if (ImGui::IsWindowAppearing()) {
        time_start = now;
        last_render_ms = 0;
    }

    const uint32_t t = now - time_start;
    const uint32_t interval_ms = 66; // ~15 FPS update rate for the offscreen logo
    if (last_render_ms == 0 || (now - last_render_ms) >= interval_ms) {
        logo_fbo->Target();
        RenderLogo(t);
        logo_fbo->Restore();
        last_render_ms = now;
        last_logo_time = t;
    } else {
        (void)last_logo_time;
    }

    ImTextureID id = (ImTextureID)(intptr_t)logo_fbo->Texture();
    float t_w = 256.0;
    float t_h = 256.0;
    float x_off = 0;
    ImGui::Image(id,
        ImVec2((t_w-x_off)*g_viewport_mgr.m_scale, t_h*g_viewport_mgr.m_scale),
        ImVec2(x_off/t_w, t_h/t_h),
        ImVec2(t_w/t_w, 0));
    return;
#else
    static uint32_t time_start = 0;
    static uint32_t offset = 0;
    uint32_t now = SDL_GetTicks();

    if (ImGui::IsWindowAppearing()) {
        time_start = now;
    }

    logo_fbo->Target();
    ImTextureID id = (ImTextureID)(intptr_t)logo_fbo->Texture();
    float t_w = 256.0;
    float t_h = 256.0;
    float x_off = 0;
    ImVec2 pos = ImGui::GetCursorPos();
    ImGui::Image(id,
        ImVec2((t_w-x_off)*g_viewport_mgr.m_scale, t_h*g_viewport_mgr.m_scale),
        ImVec2(x_off/t_w, t_h/t_h),
        ImVec2(t_w/t_w, 0));
    ImVec2 size = ImGui::GetItemRectSize();
    ImGui::SetCursorPos(pos);
    ImGui::InvisibleButton("###empty", ImVec2(size.x, size.y*0.8));
    if (ImGui::IsItemClicked()) {
        time_start = now;
        offset = 0;
    }
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 item_min = ImGui::GetItemRectMin();
        ImVec2 mouse = ImGui::GetMousePos();
        time_start = now;
        offset = 1500 * fmin(fmax(0, (mouse.x - item_min.x) / (size.x)), 1);
    }

    RenderLogo(now - time_start + offset);
    logo_fbo->Restore();
#endif
}
