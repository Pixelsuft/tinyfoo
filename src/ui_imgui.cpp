#include <ui.hpp>
#include <app.hpp>
#include <new.hpp>
#include <log.hpp>
#include <lbs.hpp>
#include <playlist.hpp>
#include <util.hpp>
#include <imgui.h>
#include <algorithm>
#include <SDL3/SDL.h>

namespace app {
    extern bool ctrl_state;
    extern bool shift_state;
}

namespace ui {
    struct UiData {
        char pl_name_buf[64];
        char* pl_path_buf;
        ImFont* font1;
        pl::Playlist* last_pl;
        pl::Playlist* need_conf_pl;
        Point size;
        bool show_about;
        bool show_playlist_conf;
    };

    UiData* data;

    pl::Playlist* get_last_pl() {
        return data->last_pl;
    }

    void draw_menubar();
    void draw_playback_buttons();
    void draw_volume_control();
    void draw_position();
    void draw_playlist_tabs();
    void draw_tab();
    void draw_meta();
    void draw_playlist_view();
    void draw_about();
    void draw_playlist_conf();
}

bool ui::init() {
    ImGuiIO& io = ImGui::GetIO();
    data = tf::bump_nw<UiData>();
    data->last_pl = nullptr;
    data->show_about = false;
    data->show_playlist_conf = false;
    data->pl_path_buf = (char*)mem::alloc(65536);
    if (!data->pl_path_buf) {
        TF_ERROR(<< "WTF failed to alloc pl_path_buf");
        tf::dl(data);
        return false;
    }
    data->font1 = io.Fonts->AddFontFromFileTTF("assets/Roboto-Regular.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    if (!data->font1) {
        TF_ERROR(<< "Failed to load ImGui Font 1");
        mem::free((void*)data->pl_path_buf);
        tf::dl(data);
        return false;
    }
    ImGui::StyleColorsDark();
    return true;
}

void ui::update_size(const Point& size) {
    data->size = size;
}

void ui::draw_menubar() {
    // Menu
    // https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp#L641
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Add files...", nullptr, nullptr)) {
            if (data->last_pl)
                pl::add_files_dialog(data->last_pl);
        }
        if (ImGui::MenuItem("Add folder...", nullptr, nullptr)) {
            if (data->last_pl)
                pl::add_folder_dialog(data->last_pl);            
        }
        ImGui::Separator();
        if (ImGui::MenuItem("New playlist...", nullptr, nullptr)) {

        }
        if (ImGui::MenuItem("Configure playlist", nullptr, nullptr)) {
            data->need_conf_pl = data->last_pl;
            data->show_playlist_conf = true;
            SDL_memcpy(data->pl_name_buf, data->need_conf_pl->name.c_str(), std::min(data->need_conf_pl->name.size() + 1, (size_t)63));
            SDL_memcpy(data->pl_path_buf, data->need_conf_pl->path.c_str(), std::min(data->need_conf_pl->path.size() + 1, (size_t)65535));
            data->pl_name_buf[63] = '\0';
            data->pl_path_buf[65535] = '\0';
        }
        if (ImGui::MenuItem("Save playlist", nullptr, nullptr)) {
            if (data->last_pl)
                pl::save(data->last_pl);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", nullptr, nullptr)) {
            app::stop();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Playback")) {
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About", nullptr, nullptr)) {
            data->show_about = true;
        }
        ImGui::EndMenu();
    }
}

void ui::draw_playback_buttons() {
    ImGui::Button("0");
    ImGui::Button(">");
    ImGui::Button("||");
    ImGui::Button("|<");
    ImGui::Button(">|");
    ImGui::Button(">?");
}

void ui::draw_volume_control() {
    static float vol = 1.f;
    ImGui::PushID("VolumeSlider");
    ImGui::PushItemWidth(100.f);
    ImGui::SliderFloat("", &vol, 0.f, 1.f, "", ImGuiSliderFlags_NoRoundToFormat);
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void ui::draw_position() {
    static float pos = 0.f;
    ImVec2 hacky_rect = ImGui::GetItemRectMax();
    ImGui::PushID("PositionSlider");
    ImGui::PushItemWidth(data->size.x - (hacky_rect.x + 16.f));
    ImGui::SliderFloat("", &pos, 0.f, 1.f, "", ImGuiSliderFlags_NoRoundToFormat);
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void ui::draw_playlist_tabs() {
    if (ImGui::BeginTabBar("PlaylistTabs", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton |
        ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_DrawSelectedOverline)) {
        data->last_pl = nullptr;
        for (auto it = pl::pls->begin(); it != pl::pls->end(); it++) {
            if (ImGui::BeginTabItem((*it)->name.c_str(), nullptr, 0)) {
                data->last_pl = *it;
                draw_tab();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}

void ui::draw_meta() {
    ImGui::Button("Test1 btn?");
    ImGui::Button("Test2 btn");
    ImGui::Button("Test3 btn");
    ImGui::Button("Test4 btn");
    ImGui::Button("Test 2 btn!!!!!");
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %f", 1.f / io.DeltaTime);
}

void ui::draw_playlist_view() {
    if (!data->last_pl)
        return;
    if (ImGui::BeginTable("PlaylistTable", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("File Name");
        ImGui::TableSetupColumn("Duration");
        ImGui::TableSetupColumn("Codec");
        ImGui::TableSetupColumn("Bitrate");
        ImGui::TableSetupColumn("Last Modified");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        ImGuiListClipper clipper;
        clipper.Begin((int)data->last_pl->mus.size());
        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                ImGui::TableNextRow();
                audio::Music* mus = data->last_pl->mus[row];
                // fn, dur, codec, bitrate, last mod
                ImGui::TableSetColumnIndex(0);
                bool ret = ImGui::Selectable(mus->fn.c_str(), &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                if (mus->selected)
                    ImGui::OpenPopupOnItemClick("MusSelPopup", ImGuiPopupFlags_MouseButtonRight);
                else if (!ImGui::IsPopupOpen("MusSelPopup")) {
                    // TODO: support switching select
                    ImGui::OpenPopupOnItemClick("MusSelPopup", ImGuiPopupFlags_MouseButtonRight);
                    if (ImGui::IsPopupOpen("MusSelPopup")) {
                        for (auto it = data->last_pl->selected.begin(); it != data->last_pl->selected.end(); it++) {
                            data->last_pl->mus[*it]->selected = false;
                        }
                        data->last_pl->selected.clear();
                        data->last_pl->selected.push_back(row);
                        mus->selected = true;
                    }
                }
                ImGui::TableSetColumnIndex(1);
                char dur_buf[11];
                int rounded_dur = (int)SDL_floorf(mus->dur);
                int need_secs = rounded_dur % 60;
                SDL_snprintf(dur_buf, 11, (need_secs < 10) ? "%i:0%i" : "%i:%i", rounded_dur / 60, need_secs);
                ImGui::Selectable(dur_buf, &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableSetColumnIndex(2);
                ImGui::Selectable("TODO", &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableSetColumnIndex(3);
                ImGui::Selectable("TODO", &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableSetColumnIndex(4);
                ImGui::Selectable("TODO", &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ret) {
                    bool pushed = false;
                    if (app::ctrl_state) {
                        // ...
                    }
                    else if (app::shift_state) {
                        int min_id = data->last_pl->last_shift_sel;
                        int max_id = data->last_pl->last_shift_sel2;
                        for (int i = min_id; i <= max_id; i++) {
                            auto it = std::find(data->last_pl->selected.begin(), data->last_pl->selected.end(), i);
                            if (it != data->last_pl->selected.end()) {
                                data->last_pl->selected.erase(it);
                                data->last_pl->mus[i]->selected = false;
                            }
                        }
                        min_id = std::min(data->last_pl->last_sel, row);
                        max_id = std::max(data->last_pl->last_sel, row);
                        for (int i = min_id; i <= max_id; i++) {
                            if (std::find(data->last_pl->selected.begin(), data->last_pl->selected.end(), i) == data->last_pl->selected.end()) {
                                data->last_pl->mus[i]->selected = true;
                                data->last_pl->selected.push_back(i);
                                // data->last_pl->last_sel = i;
                            }
                        }
                        data->last_pl->last_shift_sel = min_id;
                        data->last_pl->last_shift_sel2 = max_id;
                        pushed = true;
                    }
                    else {
                        for (auto it = data->last_pl->selected.begin(); it != data->last_pl->selected.end(); it++) {
                            data->last_pl->mus[*it]->selected = false;
                        }
                        data->last_pl->selected.clear();
                    }
                    if (mus->selected) {
                        if (!pushed) {
                            data->last_pl->selected.push_back(row);
                            if (app::shift_state)
                                data->last_pl->last_shift_sel = row;
                            else
                                data->last_pl->last_sel = row;
                        }
                    }
                    else if (!pushed) {
                        if (app::ctrl_state) {
                            for (auto it = data->last_pl->selected.begin(); it != data->last_pl->selected.end(); it++) {
                                if ((*it) == row) {
                                    data->last_pl->selected.erase(it);
                                    break;
                                }
                            }
                        }
                        else {
                            mus->selected = true;
                            if (std::find(data->last_pl->selected.begin(), data->last_pl->selected.end(), row) == data->last_pl->selected.end())
                                data->last_pl->selected.push_back(row);
                        }
                        if (app::shift_state)
                            data->last_pl->last_shift_sel = row;
                        else
                            data->last_pl->last_sel = row;
                    }
                }
            }
        }
        if (ImGui::BeginPopupContextItem("MusSelPopup")) {
            if (ImGui::Button("Play")) {
                pl::play_selected(data->last_pl);
                ImGui::CloseCurrentPopup();
            }
            ImGui::Separator();
            if (ImGui::Button("Remove")) {
                pl::remove_selected(data->last_pl);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::EndTable();
    }
}

void ui::draw_tab() {
    //ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.f, 0.f });
    if (ImGui::BeginTable("PlaylistTableMain", 2, ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("PlaylistMetaCol", ImGuiTableColumnFlags_IndentDisable);
        ImGui::TableSetupColumn("PlaylistViewCol", ImGuiTableColumnFlags_IndentDisable);
        ImGui::TableNextColumn();
        draw_meta();
        ImGui::TableNextColumn();
        draw_playlist_view();
        ImGui::EndTable();
    }
    //ImGui::PopStyleVar();
}

void ui::draw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos({ 0.f, 0.f }, 1);
    ImGui::SetNextWindowSize({ data->size.x, data->size.y }, 1);
    // ImGuiWindowFlags_NoBackground ?
    if (ImGui::Begin("tinyfoo", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar)) {
        ImGui::PushFont(data->font1);
        if (ImGui::BeginMenuBar()) {
            draw_menubar();
            ImGui::Separator();
            draw_playback_buttons();
            ImGui::Separator();
            draw_volume_control();
            ImGui::Separator();
            draw_position();
            ImGui::EndMenuBar();
        }
        ImGui::PopFont();
        draw_playlist_tabs();
    }
    ImGui::End();
    ImGui::PopStyleVar();
    if (data->show_playlist_conf) {
        ImGui::SetNextWindowFocus();
        ImGui::SetNextWindowSize({ 500.f, 200.f }, ImGuiCond_Appearing);
        if (ImGui::Begin("Configure playlist", &data->show_playlist_conf, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {
            draw_playlist_conf();
        }
        ImGui::End();
    }
    if (data->show_about) {
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin("About tinyfoo", &data->show_about, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
            draw_about();
        }
        ImGui::End();
    }
}

void ui::draw_playlist_conf() {
    ImGui::InputText("Playlist name", data->pl_name_buf, 256);
    ImGui::InputText("Playlist path", data->pl_path_buf, 65536);
    // TODO: sort type
    if (ImGui::Button("Apply & Save")) {
        tf::str old_name(data->need_conf_pl->name);
        tf::str old_path(data->need_conf_pl->path);
        data->need_conf_pl->name = data->pl_name_buf;
        data->need_conf_pl->path = data->pl_path_buf;
        if (pl::save(data->need_conf_pl)) {
            if (!util::compare_paths(old_path, data->need_conf_pl->path)) {
                if (!SDL_RemovePath(pl::full_path_for_playlist(old_path).c_str()))
                    TF_ERROR(<< "Failed to remove old playlist data path (" << SDL_GetError() << ")");
            }
        }
        else {
            data->need_conf_pl->name = old_name;
            data->need_conf_pl->path = old_path;
            TF_WARN(<< "Failed to save");
        }
        data->show_playlist_conf = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        data->show_playlist_conf = false;
    }
}

void ui::draw_about() {
    ImGui::Text("tinyfoo by Pixelsuft");
}

void ui::destroy() {
    mem::free((void*)data->pl_path_buf);
    tf::bump_dl(data);
}
