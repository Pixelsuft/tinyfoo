#include <ui.hpp>
#include <app.hpp>
#include <new.hpp>
#include <log.hpp>
#include <lbs.hpp>
#include <playlist.hpp>
#include <util.hpp>
#include <vec.hpp>
#include <res.hpp>
#include <audio_base.hpp>
#include <set.hpp>
#include <imgui.h>
#include <algorithm>
#include <SDL3/SDL.h>

namespace app {
    extern void* win_handle;
    extern Point drop_pos;
    extern bool ctrl_state;
    extern bool shift_state;
    extern bool drop_state;
    extern bool can_i_drop;
}

namespace ui {
    struct UiData {
        tf::vec<tf::str> log_cache;
        tf::str meta_fn;
        tf::str meta_fmt;
        size_t meta_mod;
        double meta_dur;
        char pl_name_buf[64];
        size_t meta_sz;
        char* pl_path_buf;
        ImFont* font1;
        ImFont* font2;
        pl::Playlist* last_pl;
        pl::Playlist* sel_pl;
        pl::Playlist* need_conf_pl;
        Point size;
        bool show_about;
        bool show_logs;
        bool show_playlist_conf;
    };

    UiData* data;

    pl::Playlist* get_last_pl(int hacky) {
        if (hacky == 1 && (data->show_about || data->show_logs || data->show_playlist_conf))
            return nullptr;
        if (hacky == 2)
            return data->sel_pl ? data->sel_pl : data->last_pl;
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
    void draw_logs();
    void draw_playlist_conf();
    void update_meta_info();
    void push_log(const char* data, const char* file, const char* func, int line, int category);

    static inline void fmt_file_size(char* buf, size_t sz) {
        if (sz < 1024u) {
            SDL_itoa((int)sz, buf, 10);
            SDL_strlcat(buf, " B", 32);
        }
        else if (sz < (size_t)1024u * 1024u) {
            SDL_itoa((int)(sz >> 10), buf, 10);
            SDL_strlcat(buf, " KB", 32);
        }
        else if (sz < (size_t)1024u * 1024u * 1024u) {
            SDL_itoa((int)(sz >> 20), buf, 10);
            SDL_strlcat(buf, " MB", 32);
        }
        else if (1) {
            SDL_itoa((int)(sz >> 30), buf, 10);
            SDL_strlcat(buf, " GB", 32);
        }
    }

    static inline void fmt_last_mod(char* buf, uint64_t last_mod) {
        if (last_mod == 0)
            SDL_memcpy(buf, "Unknown", 8);
        else {
            struct tm* time_s = util::tm_from_sdl_time(last_mod);
            SDL_snprintf(
                buf, 64, "%i-%02i-%02i %02i:%02i:%02i",
                time_s->tm_year + 1900, time_s->tm_mon + 1, time_s->tm_mday,
                time_s->tm_hour, time_s->tm_min, time_s->tm_sec
            );
        }
    }

    static inline void fmt_duration(char* buf, double dur) {
        if (dur < 0.0) {
            SDL_memcpy(buf, "???", 4);
            return;
        }
        int rounded_dur = (int)SDL_floor(dur);
        if (rounded_dur < 3600)
            SDL_snprintf(buf, 32, "%i:%02i", rounded_dur / 60, rounded_dur % 60);
        else if (rounded_dur < 86400)
            SDL_snprintf(buf, 32, "%i:%02i:%02i", rounded_dur / 3600, (rounded_dur % 3600) / 60, rounded_dur % 60);
        else
            SDL_snprintf(buf, 32, "%id %i:%02i:%02i", rounded_dur / 86400, (rounded_dur % 86400) / 3600, ((rounded_dur % 86400) % 3600) / 60, rounded_dur % 60);
    }

    static inline void do_extra_stuff() {
        if (1) {
            // Useful on windows
            if (audio::au->cur_mus) {
                char pos_buf[32];
                char dur_buf[32];
                fmt_duration(dur_buf, (float)audio::au->cur_get_dur());
                fmt_duration(pos_buf, (float)audio::au->cur_get_pos());
                tf::str title = audio::au->cur_mus->fn + " [" + pos_buf + "/" + dur_buf + "]";
                SDL_SetWindowTitle((SDL_Window*)app::win_handle, title.c_str());
            }
            else {
                SDL_SetWindowTitle((SDL_Window*)app::win_handle, "tinyfoo");
            }
        }
    }

    void fix_selected_pl() {
        data->sel_pl = data->last_pl;
    }
}

bool ui::init() {
    ImGuiIO& io = ImGui::GetIO();
    data = tf::bump_nw<UiData>();
    data->last_pl = data->sel_pl = nullptr;
    data->show_about = false;
    data->show_logs = false;
    data->show_playlist_conf = false;
    data->meta_fmt.reserve(64);
    data->meta_fn.reserve(1000);
    data->pl_path_buf = (char*)mem::alloc(65536);
    if (!data->pl_path_buf) {
        TF_ERROR(<< "WTF failed to alloc pl_path_buf");
        tf::dl(data);
        return false;
    }
    data->font1 = nullptr;
    data->font2 = nullptr;
    // TODO: read font from config
    if (!data->font1) {
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        int sz_buf;
        void* font_data = res::read_asset_data("Roboto-Regular.ttf", sz_buf);
        data->font1 = io.Fonts->AddFontFromMemoryTTF(font_data, sz_buf, 16.f, &font_cfg);
        if (!data->font1) {
            TF_ERROR(<< "WTF failed to load default font");
            data->font1 = io.Fonts->AddFontDefault();
        }
        res::free_asset_data(font_data);
    }
    if (!data->font2) {
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        int sz_buf;
        void* font_data = res::read_asset_data("Roboto-Regular.ttf", sz_buf);
        data->font2 = io.Fonts->AddFontFromMemoryTTF(font_data, sz_buf, 24.f, &font_cfg);
        if (!data->font2) {
            TF_ERROR(<< "WTF failed to load default font");
            data->font2 = io.Fonts->AddFontDefault();
        }
        res::free_asset_data(font_data);
    }
    ImGui::StyleColorsDark();
    data->log_cache.reserve(LOG_CACHE_COUNT);
    return true;
}

void ui::update_size(const Point& size) {
    data->size = size;
}

void ui::draw_menubar() {
    // Menu
    // https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
    // static void DemoWindowMenuBar
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Add files...", nullptr, nullptr, data->last_pl != nullptr))
            pl::add_files_dialog(data->last_pl);
        if (ImGui::MenuItem("Add folder...", nullptr, nullptr, data->last_pl != nullptr))
            pl::add_folder_dialog(data->last_pl);
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
        if (ImGui::MenuItem("Save playlist", nullptr, nullptr, data->last_pl != nullptr))
            pl::save(data->last_pl);
        ImGui::Separator();
        if (ImGui::MenuItem("Rage Quit", nullptr, nullptr))
            app::stop(true);
        if (ImGui::MenuItem("Exit", nullptr, nullptr))
            app::stop(false);
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::BeginMenu("Sort", data->last_pl != nullptr)) {
            if (ImGui::MenuItem("None", nullptr, nullptr)) {
                // data->last_pl->reserve_sorting = false;
                data->last_pl->sorting = "none";
            }
            if (ImGui::MenuItem("Sort by file name", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "fn");
                pl::unremember_selected(data->last_pl);
                data->last_pl->sorting = "fn";
                data->last_pl->reserve_sorting = false;
            }
            if (ImGui::MenuItem("Sort by duration", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "dur");
                pl::unremember_selected(data->last_pl);
                data->last_pl->sorting = "dur";
                data->last_pl->reserve_sorting = false;
            }
            if (ImGui::MenuItem("Sort by last modified", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "mod_time");
                pl::unremember_selected(data->last_pl);
                data->last_pl->sorting = "mod_time";
                data->last_pl->reserve_sorting = false;
            }
            if (ImGui::MenuItem("Reverse", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "reverse");
                pl::unremember_selected(data->last_pl);
                data->last_pl->reserve_sorting = !data->last_pl->reserve_sorting;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Playback")) {
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("Show Logs", nullptr, nullptr))
            data->show_logs = true;
        if (ImGui::MenuItem("About", nullptr, nullptr))
            data->show_about = true;
        ImGui::EndMenu();
    }
}

void ui::draw_playback_buttons() {
    if (ImGui::Button("0"))
        audio::au->cur_stop();
    if (ImGui::Button(">")) { 
        if (audio::au->cur_paused())
            audio::au->cur_resume();
        else {
            // ?
        }
    }
    if (ImGui::Button("||")) {
        if (audio::au->cur_paused())
            audio::au->cur_resume();
        else
            audio::au->cur_pause();
    }
    ImGui::Button("|<");
    if (ImGui::Button(">|"))
        audio::au->force_play_cache();
    ImGui::Button(">?");
}

void ui::draw_volume_control() {
    ImGui::PushID("VolumeSlider");
    ImGui::PushItemWidth(100.f);
    if (ImGui::SliderFloat("", &audio::au->volume, 0.f, audio::au->max_volume, "", ImGuiSliderFlags_NoRoundToFormat))
        audio::au->update_volume();
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        audio::au->volume = 1.f;
        audio::au->update_volume();
    }
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void ui::draw_position() {
    ImVec2 hacky_rect = ImGui::GetItemRectMax();
    ImGui::PushID("PositionSlider");
    ImGui::PushItemWidth(data->size.x - (hacky_rect.x + 16.f));
    static bool hack_editing = false;
    float pos = audio::au->cur_get_pos();
    static float hack_pos = pos;
    static float hack_last_pos = pos;
    if (hack_editing)
        pos = hack_last_pos;
    if (ImGui::SliderFloat("", &pos, 0.f, audio::au->cur_get_dur(), "", ImGuiSliderFlags_NoRoundToFormat)) {
        // audio::au->cur_set_pos(pos);
        hack_editing = true;
        hack_last_pos = pos;
    }
    if (ImGui::IsItemClicked()) {
        hack_pos = pos;
        hack_editing = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        hack_editing = false;
        hack_pos = pos;
        audio::au->cur_set_pos(hack_last_pos);
    }
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void ui::draw_playlist_tabs() {
    if (ImGui::BeginTabBar("PlaylistTabs", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton |
        ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_DrawSelectedOverline)) {
        pl::Playlist* prev = data->last_pl;
        data->last_pl = nullptr;
        for (auto it = pl::pls->begin(); it != pl::pls->end(); it++) {
            if (ImGui::BeginTabItem((*it)->name.c_str(), nullptr, 0)) {
                data->last_pl = *it;
                draw_tab();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
        if (data->last_pl != prev)
            update_meta_info();
    }
}

void ui::draw_meta() {
    // TODO: move this to new debug window
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %f", 1.f / io.DeltaTime);
    if (audio::au->cur_mus)
        ImGui::Text("Current: %s", audio::au->cur_mus->fn.c_str());
    ImGui::Text("Opened:");
    for (auto it = data->last_pl->mus.begin(); it != data->last_pl->mus.end(); it++) {
        if (audio::au->mus_opened(*it))
            ImGui::Text("%s", (*it)->fn.c_str());
    }
    ImGui::Text("Cache:");
    for (auto it = audio::au->cache.begin(); it != audio::au->cache.end(); it++) {
        ImGui::Text("%s", (*it)->fn.c_str());
    }
    // TODO: improve
    char temp_buf[64];
    ImGui::PushFont(data->font2);
    ImGui::TextColored(ImVec4(0.f, 162.f, 232.f, 255.f), "Location");
    ImGui::PopFont();
    ImGui::Text("File names: %s", data->meta_fn.c_str());
    fmt_file_size(temp_buf, data->meta_sz);
    ImGui::Text("Total size: %s", temp_buf);
    fmt_last_mod(temp_buf, data->meta_mod);
    ImGui::Text("Last modified: %s", temp_buf);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(ImVec4(0.f, 162.f, 232.f, 255.f), "General");
    ImGui::PopFont();
    ImGui::Text("Items selected: %i", (int)data->last_pl->selected.size());
    fmt_duration(temp_buf, data->meta_dur);
    ImGui::Text("Duration: %s", temp_buf);
    ImGui::Text("Formats: %s", data->meta_fmt.c_str());
}

void ui::draw_playlist_view() {
    if (ImGui::BeginTable("PlaylistTable", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY)) {
        bool something_changed = false;
        ImGui::TableSetupColumn("File Name");
        ImGui::TableSetupColumn("Duration");
        ImGui::TableSetupColumn("Format");
        ImGui::TableSetupColumn("Last Modified");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        ImGuiListClipper clipper;
        clipper.Begin((int)data->last_pl->mus.size());
        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                ImGui::TableNextRow();
                audio::Music* mus = data->last_pl->mus[row];
                ImGui::TableSetColumnIndex(0);
                bool ret = ImGui::Selectable(mus->fn.c_str(), &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                if (mus->selected)
                    ImGui::OpenPopupOnItemClick("MusSelPopup", ImGuiPopupFlags_MouseButtonRight);
                else if (!ImGui::IsPopupOpen("MusSelPopup")) {
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
                char buf[64];
                fmt_duration(buf, (double)mus->dur);
                ret |= ImGui::Selectable(buf, &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableSetColumnIndex(2);
                ret |= ImGui::Selectable(audio::get_type_str(mus->type), &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableSetColumnIndex(3);
                fmt_last_mod(buf, mus->last_mod);
                ret |= ImGui::Selectable(buf, &mus->selected, ImGuiSelectableFlags_SpanAllColumns);
                something_changed |= ret;
                if (ret) {
                    Uint64 now = SDL_GetTicks();
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
                    if ((now - mus->last_click) <= 250) {
                        if (app::ctrl_state) {
                            mus->selected = true;
                            if (std::find(data->last_pl->selected.begin(), data->last_pl->selected.end(), row) == data->last_pl->selected.end())
                                data->last_pl->selected.push_back(row);
                        }
                        pl::play_selected(data->last_pl);
                    }
                    mus->last_click = now;
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
        if (something_changed)
            update_meta_info();
    }
    if (app::drop_state) {
        ImVec2 ds = ImGui::GetItemRectSize();
        ImVec2 dp = ImGui::GetItemRectMin();
        app::can_i_drop = (app::drop_pos.x >= dp.x && app::drop_pos.x < (dp.x + ds.x)) && (app::drop_pos.y >= dp.y && app::drop_pos.y < (dp.y + ds.y));
        if (app::can_i_drop && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern)) {
            ImGui::SetDragDropPayload("DROP_FILES", nullptr, 0);
            ImGui::BeginTooltip();
            ImGui::Text("Upload files to the current playlist");
            ImGui::EndTooltip();
            ImGui::EndDragDropSource();
        }
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
    do_extra_stuff();
    ImGui::PushFont(data->font1);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos({ 0.f, 0.f }, 1);
    ImGui::SetNextWindowSize({ data->size.x, data->size.y }, 1);
    // ImGuiWindowFlags_NoBackground ?
    if (ImGui::Begin("tinyfoo", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar)) {
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
        draw_playlist_tabs();
    }
    ImGui::End();
    ImGui::PopStyleVar();
    if (data->show_playlist_conf) {
        ImGui::SetNextWindowFocus();
        ImGui::SetNextWindowSize({ 500.f, 200.f }, ImGuiCond_Appearing);
        if (ImGui::Begin("Configure playlist", &data->show_playlist_conf, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
            draw_playlist_conf();
        ImGui::End();
    }
    if (data->show_logs) {
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin("Logs", &data->show_logs, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
            draw_logs();
        ImGui::End();
    }
    if (data->show_about) {
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin("About tinyfoo", &data->show_about, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            draw_about();
        ImGui::End();
    }
    ImGui::PopFont();
}

void ui::draw_playlist_conf() {
    ImGui::InputText("Playlist name", data->pl_name_buf, 256);
    ImGui::InputText("Playlist path", data->pl_path_buf, 65536);
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
    if (ImGui::Button("Cancel"))
        data->show_playlist_conf = false;
}

void ui::draw_about() {
    ImGui::Text("tinyfoo by Pixelsuft");
}

void ui::destroy() {
    mem::free((void*)data->pl_path_buf);
    tf::bump_dl(data);
    data = nullptr;
}

void ui::update_meta_info() {
    if (!data->last_pl)
        return;
    tf::set<audio::Type> types;
    data->meta_fmt.clear();
    data->meta_fn.clear();
    data->meta_sz = 0;
    data->meta_mod = 0;
    data->meta_dur = 0.0;
    for (auto it = data->last_pl->selected.begin(); it != data->last_pl->selected.end(); it++) {
        audio::Music* m = data->last_pl->mus[*it];
        if (data->meta_fn.size() < 8000)
            data->meta_fn += m->fn + ", ";
        data->meta_sz += m->file_size;
        data->meta_dur += (double)m->dur;
        data->meta_mod = std::max(data->meta_mod, m->last_mod);
        types.insert(m->type);
    }
    for (auto it = types.begin(); it != types.end(); it++)
        data->meta_fmt += tf::str(audio::get_type_str(*it)) + ", ";
    if (data->meta_fmt.size() >= 2)
        data->meta_fmt.resize(data->meta_fmt.size() - 2);
    if (data->meta_fmt.size() >= 8000)
        data->meta_fmt += "...";
    if (data->meta_fn.size() >= 2)
        data->meta_fn.resize(data->meta_fn.size() - 2);
}

void ui::push_log(const char* msg, const char* file, const char* func, int line, int category) {
    (void)file;
    (void)func;
    (void)line;
    (void)category;
    if (!app::win_handle || !data)
        return;
    if (data->log_cache.size() >= LOG_CACHE_COUNT)
        data->log_cache.erase(data->log_cache.begin());
    data->log_cache.push_back(tf::str(msg));
}

void ui::handle_esc() {
    data->show_about = false;
    data->show_logs = false;
    data->show_playlist_conf = false;
}

void ui::draw_logs() {
    // TODO: color, disable autoscrolling
    for (auto it = data->log_cache.begin(); it != data->log_cache.end(); it++) {
        ImGui::Text("%s", (*it).c_str());
    }
    ImGui::SetScrollHereY(1.0f);
}
