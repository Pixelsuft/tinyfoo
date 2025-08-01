#include <lbs.hpp>
#if ENABLE_IMGUI
#include <ui.hpp>
#include <app.hpp>
#include <new.hpp>
#include <log.hpp>
#include <playlist.hpp>
#include <util.hpp>
#include <vec.hpp>
#include <ren.hpp>
#include <res.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <audio.hpp>
#include <set.hpp>
#include <control.hpp>
#include <imgui.h>
#include <imgui_styles.hpp>
#include <algorithm>
#include <SDL3/SDL.h>
#if DWM_STATUS_PATCH
#include <X11/Xlib.h>
// Breaks imgui
#ifdef Status
#undef Status
#endif
#endif
#define COOL_CYAN ImVec4(0.f, 162.f, 232.f, 255.f)
#define CTRL_BTN_VEC ImVec2(20.f * data->img_scale, 20.f * data->img_scale)

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
        conf::ConfData conf;
        char search_buf[256];
        tf::vec<tf::str> log_cache;
        tf::vec<int> search_res;
        tf::str search_prev_mask;
        tf::str meta_fn;
        tf::str meta_fmt;
#if WIN_TITLE_PATCH
        tf::str last_cap;
#endif
#if DWM_STATUS_PATCH
        SDL_Time dwm_last_upd;
        audio::Music* dwm_last_m;
        Display* dwm_disp;
        Window dwm_root;
#endif
        Point size;
        double meta_dur;
        char pl_name_buf[64];
        size_t meta_sz;
        char* pl_path_buf;
        ImFont* font1;
        ImFont* font2;
        pl::Playlist* prev_cache_pl;
        pl::Playlist* last_pl;
        pl::Playlist* sel_pl;
        pl::Playlist* need_conf_pl;
        void* logo_tex;
        void* icon_stop;
        void* icon_play;
        void* icon_pause;
        void* icon_back;
        void* icon_fwd;
        void* icon_rng;
        size_t meta_mod;
        float img_scale;
        bool searching;
        bool show_meta;
        bool show_app_conf;
        bool show_about;
        bool show_logs;
        bool show_playlist_conf;
        bool show_meta_debug;
        bool show_search;
    };

    UiData* data;

    pl::Playlist* get_last_pl(int hacky) {
        if (hacky == 1 && (data->searching || data->show_about || data->show_logs || data->show_playlist_conf || data->show_app_conf))
            return nullptr;
        if (hacky == 2)
            return data->sel_pl ? data->sel_pl : data->last_pl;
        if (hacky == 3) {
            pl::Playlist* ret = (data->prev_cache_pl == data->last_pl) ? nullptr : data->last_pl;
            data->prev_cache_pl = data->last_pl;
            return ret;
        }
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
    void draw_playlist_music_row(int row, bool& something_changed);
    void draw_about();
    void draw_logs();
    void draw_playlist_conf();
    void draw_settings();
    void update_meta_info();
    void update_search();
    void draw_search();
    void push_log(const char* data, const char* file, const char* func, int line, int category);

    static inline bool can_pl() {
        return data->last_pl != nullptr && !data->searching;
    }

    static inline void apply_theme(const tf::str& style) {
        style_reset();
        if (style == "dark")
            ImGui::StyleColorsDark();
        else if (style == "light")
            ImGui::StyleColorsLight();
        else if (style == "classic")
            ImGui::StyleColorsClassic();
        else if (style == "gpulib")
            style_gpulib();
        else if (style == "dracula")
            style_dracula();
        else if (style == "ue4")
            style_ue4();
        else if (style == "cherry")
            style_cherry();
        else if (style == "adobe")
            style_adobe();
        else if (style == "vgui")
            style_vgui();
        else if (style == "gold")
            style_gold();
        else if (style == "sonic_riders")
            style_sonic_riders();
        else if (style == "visual_studio")
            style_visual_studio();
        else if (style == "green_font")
            style_green_font();
        else if (style == "red_font")
            style_red_font();
        else if (style == "deep_dark")
            style_deep_dark();
        else if (style == "mediacy")
            style_mediacy();
        else if (style == "duck_red")
            style_duck_red();
        else if (style == "ruda")
            style_ruda();
        else if (style == "darky")
            style_darky();
        else if (style == "discord")
            style_discord();
        else if (style == "enemymouse")
            style_enemymouse();
        else if (style == "material_flat")
            style_material_flat();
        else if (style == "windark")
            style_windark();
        else if (style == "photoshop")
            style_photoshop();
        else {
            TF_WARN(<< "Unknown imgui style \"" << style << "\"");
            ImGui::StyleColorsDark();
        }
    }

    static inline void fmt_file_size(char* buf, size_t sz) {
        // TODO: maybe rounding?
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
        size_t rounded_dur = (int)SDL_floor(dur);
        if (rounded_dur < 3600)
            SDL_snprintf(buf, 32, "%i:%02i", (int)rounded_dur / 60, (int)rounded_dur % 60);
        else if (rounded_dur < 86400)
            SDL_snprintf(buf, 32, "%i:%02i:%02i", (int)rounded_dur / 3600, ((int)rounded_dur % 3600) / 60, (int)rounded_dur % 60);
        else
            SDL_snprintf(buf, 32, "%id %i:%02i:%02i", (int)(rounded_dur / 86400), (int)((rounded_dur % 86400) / 3600), (int)(((rounded_dur % 86400) % 3600) / 60), (int)(rounded_dur % 60));
    }

    void fix_selected_pl() {
        data->sel_pl = data->last_pl;
    }
}

void ui::do_extra_stuff() {
#if WIN_TITLE_PATCH
    tf::str new_cap;
    if (!audio::au->cur_stopped()) {
        char pos_buf[32];
        char dur_buf[32];
        fmt_duration(dur_buf, (float)audio::au->cur_get_dur());
        fmt_duration(pos_buf, (float)audio::au->cur_get_pos());
        new_cap = audio::au->cur_mus->fn + " [" + pos_buf + "/" + dur_buf + "] [tinyfoo]";
    }
    else
        new_cap = "tinyfoo";
    if (new_cap != data->last_cap) {
        data->last_cap = new_cap;
        SDL_SetWindowTitle((SDL_Window*)app::win_handle, new_cap.c_str());
    }
#endif
#if DWM_STATUS_PATCH
    SDL_Time ticks;
    if (data->dwm_last_m != audio::au->cur_mus) {
        data->dwm_last_m = audio::au->cur_mus;
        data->dwm_last_upd = 0; // Hack
    }
    if (SDL_GetCurrentTime(&ticks)) {
        if ((ticks / 1000000000) != data->dwm_last_upd) {
            char buf[DWM_STATUS_BUF_SIZE];
            data->dwm_last_upd = ticks / 1000000000;
            struct tm* time_s = util::tm_from_sdl_time(ticks);
            if (audio::au->cur_stopped()) {
                SDL_snprintf(
                    buf, DWM_STATUS_BUF_SIZE, "%i-%02i-%02i %02i:%02i:%02i",
                    time_s->tm_year + 1900, time_s->tm_mon + 1, time_s->tm_mday,
                    time_s->tm_hour, time_s->tm_min, time_s->tm_sec
                );
            }
            else {
                char pos_buf[32];
                char dur_buf[32];
                fmt_duration(dur_buf, (float)audio::au->cur_get_dur());
                fmt_duration(pos_buf, (float)audio::au->cur_get_pos());
                SDL_snprintf(
                    buf, DWM_STATUS_BUF_SIZE, "%s [%s/%s] | %i-%02i-%02i %02i:%02i:%02i",
                    audio::au->cur_mus->fn.substr(0, DWM_STATUS_FN_SIZE).c_str(), pos_buf, dur_buf,
                    time_s->tm_year + 1900, time_s->tm_mon + 1, time_s->tm_mday,
                    time_s->tm_hour, time_s->tm_min, time_s->tm_sec
                );
            }
            XStoreName(data->dwm_disp, data->dwm_root, buf);
            XFlush(data->dwm_disp);
        }
    }
    else
        TF_WARN(<< "Failed to get current time (" << SDL_GetError() << ")");
#endif
}

bool ui::init() {
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;  // We don't need it
    data = tf::bump_nw<UiData>();
    data->last_pl = data->sel_pl = data->prev_cache_pl = data->need_conf_pl = nullptr;
    data->search_buf[0] = '\0';
    data->show_about = false;
    data->show_logs = false;
    data->show_playlist_conf = false;
    data->show_app_conf = false;
    data->searching = false;
    data->show_search = false;
    data->show_meta = conf::read_bool("imgui", "show_meta", true);
    data->show_meta_debug = !IS_RELEASE;
    data->img_scale = 1.f;
    data->logo_tex = ren::rn->tex_from_io(res::get_asset_io("icon.png"), true);
    data->icon_stop = ren::rn->tex_from_io(res::get_asset_io("icon_stop.png"), true);
    data->icon_play = ren::rn->tex_from_io(res::get_asset_io("icon_play.png"), true);
    data->icon_pause = ren::rn->tex_from_io(res::get_asset_io("icon_pause.png"), true);
    data->icon_back = ren::rn->tex_from_io(res::get_asset_io("icon_back.png"), true);
    data->icon_fwd = ren::rn->tex_from_io(res::get_asset_io("icon_fwd.png"), true);
    data->icon_rng = ren::rn->tex_from_io(res::get_asset_io("icon_rng.png"), true);
    data->meta_fmt.reserve(64);
    data->meta_fn.reserve(1000);
    data->pl_path_buf = (char*)mem::alloc(65536);
    if (!data->pl_path_buf) {
        TF_ERROR(<< "WTF failed to alloc pl_path_buf");
        tf::dl(data);
        return false;
    }
    // TODO: more fonts for different stuff maybe???
    float font1_size = 16.f;
    float font2_size = 24.f;
    data->font1 = nullptr;
    data->font2 = nullptr;
    font1_size = conf::read_float("imgui", "font1_size", 16.f);
    font2_size = conf::read_float("imgui", "font2_size", 24.f);
    tf::str font1_path = conf::read_str("imgui", "font1_path", "");
    if (font1_path.size() > 0) {
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        size_t sz_buf;
        void* font_data = SDL_LoadFile(font1_path.c_str(), &sz_buf);
        if (font_data) {
            data->font1 = io.Fonts->AddFontFromMemoryTTF(font_data, (int)sz_buf, font1_size, &font_cfg);
            SDL_free(font_data);
        }
        if (!data->font1)
            TF_WARN(<< "Failed to load custom font 1 (" << SDL_GetError() << ")");
    }
    tf::str font2_path = conf::read_str("imgui", "font2_path", "");
    if (font2_path.size() > 0) {
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        size_t sz_buf;
        void* font_data = SDL_LoadFile(font2_path.c_str(), &sz_buf);
        if (font_data) {
            data->font2 = io.Fonts->AddFontFromMemoryTTF(font_data, (int)sz_buf, font2_size, &font_cfg);
            SDL_free(font_data);
        }
        if (!data->font2)
            TF_WARN(<< "Failed to load custom font 2 (" << SDL_GetError() << ")");
    }
    tf::str style_pref = conf::read_str("imgui", "style", "dark");
    data->img_scale = conf::read_float("imgui", "img_scale", 1.f);
    apply_theme(style_pref);
    if (!data->font1) {
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        int sz_buf;
        void* font_data = res::read_asset_data("Roboto-Regular.ttf", sz_buf);
        if (font_data)
            data->font1 = io.Fonts->AddFontFromMemoryTTF(font_data, sz_buf, font1_size, &font_cfg);
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
        if (font_data)
            data->font2 = io.Fonts->AddFontFromMemoryTTF(font_data, sz_buf, font2_size, &font_cfg);
        if (!data->font2) {
            TF_ERROR(<< "WTF failed to load default font");
            data->font2 = io.Fonts->AddFontDefault();
        }
        res::free_asset_data(font_data);
    }
    data->log_cache.reserve(LOG_CACHE_COUNT);
#if DWM_STATUS_PATCH
    data->dwm_last_m = nullptr;
    data->dwm_disp = XOpenDisplay(nullptr);
    if (data->dwm_disp) {
        data->dwm_root = XRootWindow(data->dwm_disp, XDefaultScreen(data->dwm_disp));
        if (!data->dwm_root)
            TF_ERROR(<< "Failed to get X display root window");
    }
    else {
        data->dwm_root = 0;
        TF_ERROR(<< "Failed to open X display");
    }
    data->dwm_last_upd = 0;
#endif
    return true;
}

void ui::update_size(const Point& size) {
    data->size = size;
}

void ui::draw_menubar() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Add files...", nullptr, nullptr, can_pl()))
            pl::add_files_dialog(data->last_pl);
        if (ImGui::MenuItem("Add folder...", nullptr, nullptr, can_pl()))
            pl::add_folder_dialog(data->last_pl);
        ImGui::Separator();
        if (ImGui::MenuItem("New playlist...", nullptr, nullptr))
            pl::add_new_pl();
        if (ImGui::MenuItem("Configure playlist", nullptr, nullptr, can_pl())) {
            data->need_conf_pl = data->last_pl;
            data->show_playlist_conf = true;
            SDL_memcpy(data->pl_name_buf, data->need_conf_pl->name.c_str(), std::min(data->need_conf_pl->name.size() + 1, (size_t)63));
            SDL_memcpy(data->pl_path_buf, data->need_conf_pl->path.c_str(), std::min(data->need_conf_pl->path.size() + 1, (size_t)65535));
            data->pl_name_buf[63] = '\0';
            data->pl_path_buf[65535] = '\0';
        }
        if (ImGui::MenuItem("Save playlist", nullptr, nullptr, can_pl()))
            pl::save(data->last_pl);
        ImGui::Separator();
        if (ImGui::MenuItem("Settings", nullptr, nullptr)) {
            data->show_app_conf = true;
            conf::begin_editing(data->conf);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Rage Quit", nullptr, nullptr))
            app::stop(true);
        if (ImGui::MenuItem("Exit", nullptr, nullptr))
            app::stop(false);
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Clear", nullptr, nullptr, data->last_pl != nullptr))
            pl::clear_selected(data->last_pl);
        if (ImGui::MenuItem("Select all", nullptr, nullptr, data->last_pl != nullptr))
            pl::select_all(data->last_pl);
        if (ImGui::BeginMenu("Selection", can_pl())) {
            if (ImGui::MenuItem("Remove", nullptr, nullptr, data->last_pl->selected.size() > 0))
                pl::remove_selected(data->last_pl);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Sort", can_pl())) {
            if (ImGui::MenuItem("None", nullptr, nullptr)) {
                // data->last_pl->reserve_sorting = false;
                data->last_pl->sorting = "none";
                pl::reload_cache(1);
            }
            if (ImGui::MenuItem("Sort by file name", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "fn");
                pl::unremember_selected(data->last_pl);
                data->last_pl->sorting = "fn";
                data->last_pl->reserve_sorting = false;
                pl::reload_cache(1);
            }
            if (ImGui::MenuItem("Sort by duration", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "dur");
                pl::unremember_selected(data->last_pl);
                data->last_pl->sorting = "dur";
                data->last_pl->reserve_sorting = false;
                pl::reload_cache(1);
            }
            if (ImGui::MenuItem("Sort by last modified", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "mod_time");
                pl::unremember_selected(data->last_pl);
                data->last_pl->sorting = "mod_time";
                data->last_pl->reserve_sorting = false;
                pl::reload_cache(1);
            }
            if (ImGui::MenuItem("Reverse", nullptr, nullptr)) {
                pl::remember_selected(data->last_pl);
                pl::sort_by(data->last_pl, "reverse");
                pl::unremember_selected(data->last_pl);
                data->last_pl->reserve_sorting = !data->last_pl->reserve_sorting;
                pl::reload_cache(1);
            }
            ImGui::EndMenu();
        }
        bool search_sel = data->show_search;
        if (ImGui::MenuItem("Search", nullptr, &search_sel, data->last_pl != nullptr))
            toggle_search();
        ImGui::Separator();
        if (ImGui::MenuItem("Remove dead items", nullptr, nullptr, can_pl()))
            pl::remove_dead(data->last_pl);
        if (ImGui::MenuItem("Scan items for changes", nullptr, nullptr, can_pl()))
            pl::scan_changes(data->last_pl);
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Playback")) {
        bool selected = audio::au->cur_stopped();
        if (ImGui::MenuItem("Stop", nullptr, &selected))
            ctrl::stop();
        selected = audio::au->cur_paused();
        if (ImGui::MenuItem("Pause", nullptr, &selected))
            ctrl::pause();
        selected = !audio::au->cur_stopped();
        if (ImGui::MenuItem("Play", nullptr, &selected))
            ctrl::play();
        if (ImGui::MenuItem("Previous", nullptr, nullptr)) {}
        if (ImGui::MenuItem("Next", nullptr, nullptr))
            ctrl::next();
        if (ImGui::MenuItem("Random", nullptr, nullptr)) {}
        ImGui::Separator();
        if (ImGui::BeginMenu("Order", data->last_pl != nullptr)) {
            bool need_order = audio::au->order_mode == 0;
            if (ImGui::MenuItem("None", nullptr, &need_order) && need_order) {
                audio::au->order_mode = 0;
                pl::reload_cache(0);
            }
            need_order = audio::au->order_mode == 1;
            if (ImGui::MenuItem("Default", nullptr, &need_order) && need_order) {
                audio::au->order_mode = 1;
                pl::reload_cache(0);
            }
            need_order = audio::au->order_mode == 2;
            if (ImGui::MenuItem("Random", nullptr, &need_order) && need_order) {
                audio::au->order_mode = 2;
                pl::reload_cache(0);
            }
#if ORDER_COOL_RNG_PATCH
            need_order = audio::au->order_mode == 3;
            if (ImGui::MenuItem("Cool Random", nullptr, &need_order) && need_order) {
                audio::au->order_mode = 3;
                pl::reload_cache(0);
            }
#endif
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
        ImGui::MenuItem("Show Debug Info", nullptr, &data->show_meta_debug);
        if (ImGui::MenuItem("Show Logs", nullptr, nullptr))
            data->show_logs = true;
        if (ImGui::MenuItem("About", nullptr, nullptr))
            data->show_about = true;
        ImGui::EndMenu();
    }
}

void ui::draw_playback_buttons() {
    // TODO: functionality
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
    if (ImGui::ImageButton("IconStop", (ImTextureID)(intptr_t)data->icon_stop, CTRL_BTN_VEC))
        ctrl::stop();
    if (ImGui::ImageButton("IconPlay", (ImTextureID)(intptr_t)data->icon_play, CTRL_BTN_VEC))
        ctrl::play();
    if (ImGui::ImageButton("IconPause", (ImTextureID)(intptr_t)data->icon_pause, CTRL_BTN_VEC))
        ctrl::pause();
    ImGui::ImageButton("IconBack", (ImTextureID)(intptr_t)data->icon_back, CTRL_BTN_VEC);
    if (ImGui::ImageButton("IconFwd", (ImTextureID)(intptr_t)data->icon_fwd, CTRL_BTN_VEC))
        ctrl::next();
    ImGui::ImageButton("IconRng", (ImTextureID)(intptr_t)data->icon_rng, CTRL_BTN_VEC);
    ImGui::PopStyleVar();
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
    static float hack_last_pos = pos;
    if (hack_editing)
        pos = hack_last_pos;
    if (ImGui::SliderFloat("", &pos, 0.f, audio::au->cur_get_dur(), "", ImGuiSliderFlags_NoRoundToFormat)) {
        // audio::au->cur_set_pos(pos);
        hack_editing = true;
        hack_last_pos = pos;
    }
    if (ImGui::IsItemClicked())
        hack_editing = true;
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        hack_editing = false;
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
                if (data->last_pl != prev && data->searching) {
                    data->search_prev_mask = "";
                    update_search();
                }
                draw_tab();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
        if (data->last_pl != prev)
            update_meta_info();
    }
}

void ui::toggle_search() {
    if (data->show_search) {
        data->search_res.clear();
        data->search_prev_mask = "";
        data->searching = false;
        data->show_search = false;
    }
    else {
        data->show_search = true;
    }
}

void ui::update_search() {
    tf::str mask(data->search_buf);
    if (data->search_prev_mask.size() > 0 && util::str_starts_with(mask, data->search_prev_mask)) {
        for (int i = (int)data->search_res.size(); i > 0; i--) {
            if (!util::str_matches_mask(data->last_pl->mus[data->search_res[i - 1]]->fn, mask))
                data->search_res.erase(data->search_res.begin() + i - 1);
        }
    }
    else {
        data->search_res.clear();
        for (auto it = data->last_pl->mus.begin(); it != data->last_pl->mus.end(); it++) {
            if (util::str_matches_mask((*it)->fn, mask))
                data->search_res.push_back((int)std::distance(data->last_pl->mus.begin(), it));
        }
    }
    data->search_prev_mask = mask;
}

void ui::draw_search() {
    if (ImGui::InputText("Search", data->search_buf, 256)) {
        if (data->search_buf[0] == '\0' && data->searching)
            ui::toggle_search();
        else if (data->search_buf[0] != '\0') {
            data->searching = true;
            update_search();
        }
    }
}

void ui::draw_meta() {
    if (data->show_search)
        draw_search();
    if (data->show_meta_debug) {
        ImGui::PushFont(data->font2);
        ImGui::TextColored(COOL_CYAN, "Debug");
        ImGui::PopFont();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("FPS: %f", 1.f / io.DeltaTime);
        if (audio::au->cur_mus)
            ImGui::Text("Current: %s", audio::au->cur_mus->fn.c_str());
        int op_cnt = 0;
        for (auto it = data->last_pl->mus.begin(); it != data->last_pl->mus.end(); it++) {
            if (audio::au->mus_opened(*it))
                op_cnt++;
        }
        ImGui::Text("Cache (%i):", (int)audio::au->cache.size());
        for (auto it = audio::au->cache.begin(); it != audio::au->cache.end(); it++)
            ImGui::TextUnformatted((*it)->fn.c_str());
        ImGui::Text("Opened (%i):", op_cnt);
        for (auto it = data->last_pl->mus.begin(); it != data->last_pl->mus.end(); it++) {
            if (audio::au->mus_opened(*it))
                ImGui::TextUnformatted((*it)->fn.c_str());
        }
        ImGui::Text("Repeat blocks (%i):", (int)data->last_pl->repeating.size());
        for (auto it = data->last_pl->repeating.begin(); it != data->last_pl->repeating.end(); it++)
            ImGui::TextUnformatted((*it)->fn.c_str());
    }
    // TODO: improve?
    char temp_buf[64];
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "Location");
    ImGui::PopFont();
    ImGui::Text("File names: %s", data->meta_fn.c_str());
    fmt_file_size(temp_buf, data->meta_sz);
    ImGui::Text("Total size: %s", temp_buf);
    fmt_last_mod(temp_buf, data->meta_mod);
    ImGui::Text("Last modified: %s", temp_buf);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "General");
    ImGui::PopFont();
    ImGui::Text("Items selected: %i", (int)data->last_pl->selected.size());
    fmt_duration(temp_buf, data->meta_dur);
    ImGui::Text("Duration: %s", temp_buf);
    ImGui::Text("Formats: %s", data->meta_fmt.c_str());
}

void ui::draw_playlist_music_row(int row, bool& something_changed) {
    audio::Music* mus = data->last_pl->mus[row];
    ImGui::TableNextRow();
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
        if ((now - mus->last_click) <= DOUBLE_CLICK_TIME) {
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

void ui::draw_playlist_view() {
    if (ImGui::BeginTable("PlaylistTable", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY)) {
        bool something_changed = false;
        ImGui::TableSetupColumn("File Name");
        ImGui::TableSetupColumn("Duration");
        ImGui::TableSetupColumn("Format");
        ImGui::TableSetupColumn("Last Modified");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        if (data->searching) {
            if (data->search_res.size() > MUSIC_SEARCH_LIMIT) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted("Too many items to display");
            }
            else {
                for (auto it = data->search_res.begin(); it != data->search_res.end(); it++)
                    draw_playlist_music_row(*it, something_changed);
            }
        }
        else {
            ImGuiListClipper clipper;
            clipper.Begin((int)data->last_pl->mus.size());
            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                    draw_playlist_music_row(row, something_changed);
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
            ImGui::TextUnformatted("Upload files to the current playlist");
            ImGui::EndTooltip();
            ImGui::EndDragDropSource();
        }
    }
}

void ui::draw_tab() {
    if (!data->show_meta) {
        if (data->show_search)
            draw_search();
        draw_playlist_view();
        return;
    }
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
    if (data->show_app_conf) {
        ImGui::SetNextWindowSize({ 640.f, 400.f }, ImGuiCond_Appearing);
        bool prev_show = data->show_app_conf;
        if (ImGui::Begin("Settings", &data->show_app_conf, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
            draw_settings();
        ImGui::End();
        if (prev_show && !data->show_app_conf)
            apply_theme(data->conf.def_style);
    }
    if (data->show_playlist_conf) {
        ImGui::SetNextWindowSize({ 500.f, 200.f }, ImGuiCond_Appearing);
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin("Configure playlist", &data->show_playlist_conf, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
            draw_playlist_conf();
        ImGui::End();
    }
    if (data->show_logs) {
        ImGui::SetNextWindowSize({ 500.f, 400.f }, ImGuiCond_Appearing);
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin("Logs", &data->show_logs, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar))
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

void ui::draw_settings() {
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        ImGui::SetWindowFocus("Settings");
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "Renderer");
    ImGui::PopFont();
    if (ImGui::BeginCombo("Driver##ren", data->conf.ren_drv.c_str())) {
        for (int i = 0; i < (int)SDL_arraysize(conf::ren_drv); i++) {
            bool is_selected = (data->conf.ren_drv == conf::ren_drv[i]) || (i == 0 && data->conf.ren_drv.size() == 0);
            if (ImGui::Selectable(conf::ren_drv[i], &is_selected))
                data->conf.ren_drv = conf::ren_drv[i];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Checkbox("VSync", &data->conf.bools[0]);
    ImGui::Checkbox("VSync Fix (Windows)", &data->conf.bools[14]);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "ImGui");
    ImGui::PopFont();
    if (ImGui::BeginCombo("Style", data->conf.style.c_str())) {
        for (int i = 0; i < (int)SDL_arraysize(conf::ig_style_list); i++) {
            bool is_selected = (data->conf.style == conf::ig_style_list[i]);
            if (ImGui::Selectable(conf::ig_style_list[i], &is_selected))
                data->conf.style = conf::ig_style_list[i];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    apply_theme(data->conf.style);
    ImGui::Checkbox("Show Meta Tab", &data->conf.bools[16]);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "Audio");
    ImGui::PopFont();
    if (ImGui::BeginCombo("Backend", data->conf.au_bk.c_str())) {
        for (int i = 0; i < (int)SDL_arraysize(conf::au_bk); i++) {
            bool is_selected = (data->conf.au_bk == conf::au_bk[i]) || (i == 0 && data->conf.au_bk.size() == 0);
            if (ImGui::Selectable(conf::au_bk[i], &is_selected))
                data->conf.au_bk = conf::au_bk[i];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    // Assuming that 'default' device always exists
    if (ImGui::BeginCombo("Device", data->conf.dev_names[data->conf.ints[4]].c_str())) {
        for (int i = 0; i < (int)data->conf.dev_names.size(); i++) {
            bool is_selected = i == data->conf.ints[4];
            if (ImGui::Selectable(data->conf.dev_names[i].c_str(), &is_selected))
                data->conf.ints[4] = i;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::InputFloat("Volume (%)", &data->conf.floats[3]))
        data->conf.floats[3] = tf::clamp(data->conf.floats[3], 0.f, audio::au->max_volume * 100.f);
    if (ImGui::InputFloat("Max Volume (%)", &data->conf.floats[8]))
        data->conf.floats[8] = tf::clamp(data->conf.floats[8], 0.f, 10000.f);
    if (ImGui::InputFloat("Next Fading Time (ms)", &data->conf.floats[4]))
        data->conf.floats[4] = tf::clamp(data->conf.floats[4], 0.f, 10000.f);
    if (ImGui::InputFloat("Stop Fading Time (ms)", &data->conf.floats[5]))
        data->conf.floats[5] = tf::clamp(data->conf.floats[5], 0.f, 10000.f);
    if (ImGui::InputFloat("Pause Fading Time (ms)", &data->conf.floats[6]))
        data->conf.floats[6] = tf::clamp(data->conf.floats[6], 0.f, 10000.f);
    if (ImGui::InputFloat("Resume Fading Time (ms)", &data->conf.floats[7]))
        data->conf.floats[7] = tf::clamp(data->conf.floats[7], 0.f, 10000.f);
    if (ImGui::InputInt("RNG repeat blocks", &data->conf.ints[10]))
        data->conf.ints[10] = tf::clamp(data->conf.ints[10], 0, 1000);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "SDL2_mixer");
    ImGui::PopFont();
    if (ImGui::BeginCombo("Driver##sdl2", data->conf.sdl2_drv.c_str())) {
        for (int i = 0; i < (int)SDL_arraysize(conf::sdl2_drv); i++) {
            bool is_selected = data->conf.sdl2_drv == conf::sdl2_drv[i];
            if (ImGui::Selectable(conf::sdl2_drv[i], &is_selected))
                data->conf.sdl2_drv = conf::sdl2_drv[i];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Format##sdl2", data->conf.sdl2_fmt.c_str())) {
        for (int i = 0; i < (int)SDL_arraysize(conf::sdl2_fmt); i++) {
            bool is_selected = data->conf.sdl2_fmt == conf::sdl2_fmt[i];
            if (ImGui::Selectable(conf::sdl2_fmt[i], &is_selected))
                data->conf.sdl2_fmt = conf::sdl2_fmt[i];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::InputInt("Channels##sdl2", &data->conf.ints[0]))
        data->conf.ints[0] = tf::clamp(data->conf.ints[0], 0, 64);
    if (ImGui::InputInt("Frequency##sdl2", &data->conf.ints[1], 100, 10000))
        data->conf.ints[1] = tf::clamp(data->conf.ints[1], 0, 96000);
    if (ImGui::InputInt("Chunk Size##sdl2", &data->conf.ints[2], 32, 256))
        data->conf.ints[2] = tf::clamp(data->conf.ints[2], 0, 1024 * 1024);
    ImGui::Checkbox("FLAC", &data->conf.bools[1]);
    ImGui::SameLine();
    ImGui::Checkbox("MOD", &data->conf.bools[2]);
    ImGui::SameLine();
    ImGui::Checkbox("MP3", &data->conf.bools[3]);
    ImGui::SameLine();
    ImGui::Checkbox("OGG", &data->conf.bools[4]);
    ImGui::Checkbox("MID", &data->conf.bools[5]);
    ImGui::SameLine();
    ImGui::Checkbox("OPUS", &data->conf.bools[6]);
    ImGui::SameLine();
    ImGui::Checkbox("WAVPACK", &data->conf.bools[7]);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "FMOD");
    ImGui::PopFont();
    if (ImGui::BeginCombo("Driver##fmod", data->conf.fmod_drv.c_str())) {
        for (int i = 0; i < (int)SDL_arraysize(conf::fmod_drv); i++) {
            bool is_selected = data->conf.fmod_drv == conf::fmod_drv[i];
            if (ImGui::Selectable(conf::fmod_drv[i], &is_selected))
                data->conf.fmod_drv = conf::fmod_drv[i];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::InputInt("Version", &data->conf.ints[5]))
        data->conf.ints[5] = tf::clamp(data->conf.ints[5], 0, 0x00090900);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "BASS");
    ImGui::PopFont();
    if (ImGui::InputInt("Frequency##bass", &data->conf.ints[3], 100, 10000))
        data->conf.ints[3] = tf::clamp(data->conf.ints[3], 0, 96000);
    ImGui::Checkbox("Force 16-Bit", &data->conf.bools[8]);
    ImGui::SameLine();
    ImGui::Checkbox("Force Stereo", &data->conf.bools[9]);
    ImGui::SameLine();
    ImGui::Checkbox("Force DMix", &data->conf.bools[10]);
    ImGui::Checkbox("Force AudioTrack", &data->conf.bools[11]);
    ImGui::SameLine();
    ImGui::Checkbox("Force DSound", &data->conf.bools[12]);
    ImGui::SameLine();
    ImGui::Checkbox("Force Software", &data->conf.bools[13]);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "SoLoud");
    ImGui::PopFont();
    if (ImGui::BeginCombo("Driver##sl", data->conf.soloud_drv.c_str())) {
        for (int i = 0; i < (int)SDL_arraysize(conf::soloud_drv); i++) {
            bool is_selected = data->conf.soloud_drv == conf::soloud_drv[i];
            if (ImGui::Selectable(conf::soloud_drv[i], &is_selected))
                data->conf.soloud_drv = conf::soloud_drv[i];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::InputInt("Channels##sl", &data->conf.ints[7]))
        data->conf.ints[7] = tf::clamp(data->conf.ints[7], 0, 64);
    if (ImGui::InputInt("Frequency##sl", &data->conf.ints[8], 100, 10000))
        data->conf.ints[8] = tf::clamp(data->conf.ints[8], 0, 96000);
    if (ImGui::InputInt("Chunk Size##sl", &data->conf.ints[9], 32, 256))
        data->conf.ints[9] = tf::clamp(data->conf.ints[9], 0, 1024 * 1024);
    ImGui::PushFont(data->font2);
    ImGui::TextColored(COOL_CYAN, "VLC");
    ImGui::PopFont();
    ImGui::Checkbox("Fast Seeking", &data->conf.bools[15]);
    ImGui::TextUnformatted("WARN: most of the changes require restarting");
    if (ImGui::Button("Save and Close")) {
        data->show_app_conf = false;
        conf::end_editing(data->conf);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
        data->show_app_conf = false;
}

void ui::draw_playlist_conf() {
    ImGui::InputText("Playlist name", data->pl_name_buf, 256);
    ImGui::InputText("Playlist path", data->pl_path_buf, 65536);
    // TODO: validate name and path
    bool can = data->pl_name_buf[0] != '\0' && data->pl_path_buf[0] != '\0' && SDL_strcmp(data->pl_name_buf, "Unknown") != 0;
    for (auto it = pl::pls->begin(); it != pl::pls->end(); it++) {
        if ((*it) == data->need_conf_pl)
            continue;
        if ((*it)->name == data->pl_name_buf || util::compare_paths((*it)->path, data->pl_path_buf)) {
            can = false;
            break;
        }
    }
    if (can && ImGui::Button("Apply & Save")) {
        tf::str old_name(data->need_conf_pl->name);
        tf::str old_path(data->need_conf_pl->path);
        data->need_conf_pl->name = data->pl_name_buf;
        data->need_conf_pl->path = data->pl_path_buf;
        data->need_conf_pl->changed = true;
        if (pl::save(data->need_conf_pl)) {
            // Hacky
            conf::begin_editing(data->conf);
            conf::end_editing(data->conf);
            conf::request_save();
            if (old_name != "Unknown" && !util::compare_paths(old_path, data->need_conf_pl->path)) {
                if (!SDL_RemovePath(pl::full_path_for_playlist(old_path).c_str()))
                    TF_WARN(<< "Failed to remove old playlist data file (" << SDL_GetError() << ")");
            }
        }
        else {
            data->need_conf_pl->name = old_name;
            data->need_conf_pl->path = old_path;
            TF_WARN(<< "Failed to save playlist");
        }
        data->show_playlist_conf = false;
    }
    if (can)
        ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        // Should I delay that to app quit so rage quit works???
        pl::remove_pl(data->need_conf_pl);
        conf::begin_editing(data->conf);
        conf::end_editing(data->conf);
        conf::request_save();
        data->show_playlist_conf = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
        data->show_playlist_conf = false;
}

void ui::draw_about() {
    ImGui::Image((ImTextureID)(intptr_t)data->logo_tex, ImVec2(128.f * data->img_scale, 128.f * data->img_scale));
    ImGui::PushFont(data->font2);
    ImGui::TextUnformatted("Tinyfoo");
    ImGui::PopFont();
    ImGui::TextUnformatted("Created by Pixelsuft");
}

void ui::destroy() {
#if DWM_STATUS_PATCH
    if (data->dwm_disp)
        XCloseDisplay(data->dwm_disp);
#endif
    mem::free((void*)data->pl_path_buf);
    ren::rn->tex_destroy(data->icon_stop);
    ren::rn->tex_destroy(data->icon_play);
    ren::rn->tex_destroy(data->icon_pause);
    ren::rn->tex_destroy(data->icon_back);
    ren::rn->tex_destroy(data->icon_fwd);
    ren::rn->tex_destroy(data->logo_tex);
    ren::rn->tex_destroy(data->icon_rng);
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
    static const char* cat_map[] = { "INFO", "WARN", "ERROR", "FATAL" };
    if (data->log_cache.size() >= LOG_CACHE_COUNT)
        data->log_cache.erase(data->log_cache.begin());
    data->log_cache.push_back(tf::str("[") + cat_map[category] + "] " + tf::str(msg));
}

bool ui::handle_esc() {
    bool ret = data->show_app_conf | data->show_about | data->show_logs | data->show_playlist_conf | data->show_search;
    if (data->show_app_conf)
        apply_theme(data->conf.def_style);
    data->show_app_conf = false;
    data->show_about = false;
    data->show_logs = false;
    data->show_playlist_conf = false;
    data->show_search = false;
    return ret;
}

void ui::draw_logs() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 0.f });
    for (auto it = data->log_cache.begin(); it != data->log_cache.end(); it++)
        ImGui::TextUnformatted((*it).c_str());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::PopStyleVar();
}
#endif
