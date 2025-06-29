#include <app.hpp>
#include <ren.hpp>
#include <log.hpp>
#include <new.hpp>
#include <lbs.hpp>
#include <ui.hpp>
#include <res.hpp>
#include <image.hpp>
#include <audio_base.hpp>
#include <conf.hpp>
#include <playlist.hpp>
#include <SDL3/SDL.h>
#if IS_IMGUI
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#endif

namespace logger {
    extern int log_level;
    extern int ui_log_level;
}

namespace mem {
    extern Uint8* bump_ptr;
}

namespace ui {
    pl::Playlist* get_last_pl(int hacky);
}

namespace app {
    void process_event(const SDL_Event& ev);

    struct AppData {
        toml::value conf;
        tf::vec<pl::Playlist*> playlists_vec;
        tf::str base_path;
        tf::str data_path;
        tf::str conf_path;
        SDL_Window* win;
        Uint8* orig_bump;
        int stage;
        bool should_save_conf;
        bool should_play_sel_hack;
        bool rage_quit;
        bool running;
    };

    // TODO: global boolshit cleanup
    AppData* data;
    void* win_handle;
    Point drop_pos;
    bool shift_state;
    bool ctrl_state;
    bool drop_state;
    bool can_i_drop;

    void read_config();

    tf::str get_data_path() {
        return data->data_path;
    }
}

bool app::init() {
    shift_state = ctrl_state = drop_state = can_i_drop = false;
    logger::log_level = logger::ui_log_level = 0;
    Uint8* temp_bump = mem::bump_ptr = (Uint8*)SDL_malloc(BUMP_SIZE);
    if (!mem::bump_ptr) {
        TF_FATAL(<< "Failed to create bump allocator");
        return false;
    }
    data = tf::bump_nw<AppData>();
    data->orig_bump = temp_bump;
    data->stage = 0;
    data->should_play_sel_hack = false;
    data->should_save_conf = false;
    data->rage_quit = false;
    data->running = false;
    pl::pls = &data->playlists_vec;
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, "tinyfoo the music player");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, "1.0.0");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, "com.pixelsuft.tinyfoo");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "Pixelsuft");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Pixelsuft, 2025");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, "https://github.com/Pixelsuft/tinyfoo");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "mediaplayer");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        TF_FATAL(<< "Failed to init SDL (" << SDL_GetError() << ")");
        destroy();
        return false;
    }
    const char* temp_path = SDL_GetBasePath();
    if (temp_path) {
        data->base_path = tf::str(temp_path);
        TF_INFO(<< "App base path: " << data->base_path);
    }
    else
        TF_WARN(<< "Failed to get app base path (" << SDL_GetError() << ")");
    temp_path = SDL_GetPrefPath(nullptr, "tinyfoo");
    if (temp_path) {
        data->data_path = tf::str(temp_path);
        SDL_free((void*)temp_path);
        TF_INFO(<< "App data path: " << data->data_path);
    }
    else
        TF_WARN(<< "Failed to get app data path (" << SDL_GetError() << ")");
    read_config();
    data->stage = 1;
    data->win = SDL_CreateWindow(
        "tinyfoo",
        1024, 768,
        SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
    );
    win_handle = (void*)data->win;
    if (!data->win) {
        TF_FATAL(<< "Failed to create SDL Window (" << SDL_GetError() << ")");
        destroy();
        return false;
    }
    data->stage = 2;
    if (!SDL_SetWindowMinimumSize(data->win, 640, 480))
        TF_WARN(<< "Failed to set window minimum size (" << SDL_GetError() << ")");
#ifdef IS_IMGUI
    // Hacky
    ImGui::SetAllocatorFunctions((ImGuiMemAllocFunc)SDL_malloc, (ImGuiMemFreeFunc)SDL_free, nullptr);
#endif
    if (!ren::init(data->win)) {
        destroy();
        return false;
    }
    data->stage = 3;
    if (!img::init()) {
        TF_FATAL(<< "Image module init failed");
        destroy();
        return false;
    }
    if (1) {
        SDL_Surface* icon = (SDL_Surface*)img::surf_from_io(res::get_asset_io("icon.png"), true);
        if (icon && !(icon->w == 2 && icon->h == 2) && !SDL_SetWindowIcon(data->win, icon))
            TF_WARN(<< "Failed to set window icon (" << SDL_GetError() << ")");
        if (icon)
            SDL_DestroySurface(icon);
    }
    // This one actually never fails
    ui::init();
    img::destroy(); // Hack: we don't need it anymore actually
    data->stage = 4;
    audio::au = nullptr;
    if (data->conf.contains("audio") && data->conf.at("audio").is_table()) {
        toml::value au_tab = data->conf.at("audio");
        auto backend_s = toml::find_or<tf::str>(au_tab, "backend", "");
        if (0) {}
#if ENABLE_SDL2_MIXER
        else if (backend_s == "sdl2_mixer_ext")
            audio::au = audio::create_sdl2_mixer(true);
        else if (backend_s == "sdl2_mixer")
            audio::au = audio::create_sdl2_mixer(false);
#endif
#if ENABLE_FMOD
        else if (backend_s == "fmod")
            audio::au = audio::create_fmod();
#endif
#if ENABLE_BASS
        else if (backend_s == "bass")
            audio::au = audio::create_bass();
#endif
    }
    if (!audio::au || !audio::au->inited || !audio::au->dev_open()) {
        if (audio::au)
            audio::free_audio(audio::au);
        audio::au = audio::create_base();
        TF_WARN(<< "Using dummy audio library");
    }
    audio::au->update_volume();
    pl::load_playlists();
    if (1) {
        SDL_Time ticks;
        if (SDL_GetCurrentTime(&ticks)) {
            SDL_srand((Uint64)ticks);
        }
        else
            TF_ERROR(<< "Failed to get current time (" << SDL_GetError() << ")");
    }
    return true;
}

void app::process_event(const SDL_Event& ev) {
#ifdef IS_IMGUI
    ImGui_ImplSDL3_ProcessEvent(&ev);
#endif
    switch (ev.type) {
        case SDL_EVENT_QUIT: {
            if (ui::handle_esc())
                break;
            data->running = false;
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            ui::update_size(ren::get_size());
            break;
        }
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            ctrl_state = (ev.key.mod & SDL_KMOD_CTRL) != 0;
            shift_state = (ev.key.mod & SDL_KMOD_SHIFT) != 0;
            // TODO: break if other windows are visible (ex. playlist config)
            if (ev.key.down && ev.key.repeat == 0) {
                if ((ev.key.scancode == SDL_SCANCODE_RETURN || ev.key.scancode == SDL_SCANCODE_RETURN2) && ui::get_last_pl(1))
                    data->should_play_sel_hack = true;
                else if (ev.key.scancode == SDL_SCANCODE_DELETE && ui::get_last_pl(1))
                    pl::remove_selected(ui::get_last_pl(0));
                else if (ev.key.scancode == SDL_SCANCODE_A && ctrl_state && ui::get_last_pl(1))
                    pl::select_all(ui::get_last_pl(0));
                else if (ev.key.scancode == SDL_SCANCODE_C && ctrl_state && ui::get_last_pl(1)) {
                    pl::Playlist* p = ui::get_last_pl(0);
                    if (p->selected.size() == 0)
                        break;
                    tf::str copy_str = tf::str("\"") + p->mus[p->selected[0]]->full_path + "\"";
                    for (auto it = p->selected.begin() + 1; it != p->selected.end(); it++) {
                        copy_str += tf::str(" \"") + p->mus[*it]->full_path + "\"";
                    }
                    if (!SDL_SetClipboardText(copy_str.c_str()))
                        TF_WARN(<< "Failed to update clipboard (" << SDL_GetError() << ")");
                }
                else if (ev.key.scancode == SDL_SCANCODE_V && ctrl_state && ui::get_last_pl(1)) {
                    // TODO: WAIT UNTIL SDL3 FINALLY MAKES A PROPER CLIPBOARD API!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                else if (ev.key.scancode == SDL_SCANCODE_ESCAPE) {
                    ui::handle_esc();
                }
            }
        }
        case SDL_EVENT_CLIPBOARD_UPDATE: {
            break;
        }
        case SDL_EVENT_DROP_BEGIN: {
            // TODO: WAIT UNTIL SDL3 FINALLY MAKES A PROPER D&D API!!!!!!!!!!!!!!!!!!!!!!!!!!!
            drop_state = true;
            can_i_drop = false;
            drop_pos = { 0.f, 0.f };
            break;
        }
        case SDL_EVENT_DROP_POSITION: {
            drop_pos = ren::point_win_to_ren({ ev.drop.x, ev.drop.y });
            break;
        }
        case SDL_EVENT_DROP_COMPLETE: {
            drop_pos = ren::point_win_to_ren({ ev.drop.x, ev.drop.y });
            drop_state = false;
            can_i_drop = false;
            break;
        }
        case SDL_EVENT_DROP_FILE: {
            if (!can_i_drop)
                break;
            drop_pos = ren::point_win_to_ren({ ev.drop.x, ev.drop.y });
            if (ui::get_last_pl(1))
                pl::add_file_by_fp(ui::get_last_pl(0), ev.drop.data);
            break;
        }
        case SDL_EVENT_DROP_TEXT: {
            if (!can_i_drop)
                break;
            const char* src = ev.drop.source ? ev.drop.source : "nullptr";
            TF_INFO(<< "TODO: drop text " << ev.drop.data << " from " << src);
            break;
        }
    }
}

void app::run() {
    SDL_Event ev;
    ui::update_size(ren::get_size());
    data->running = true;
    SDL_ShowWindow(data->win);
    while (data->running) {
        while (SDL_PollEvent(&ev)) {
            app::process_event(ev);
        }
        audio::au->update();
        pl::update_cache();
        ren::begin_frame();
        if (!(SDL_GetWindowFlags(data->win) & SDL_WINDOW_MINIMIZED))
            ui::draw();
        else
            ui::do_extra_stuff(); // Hack
        if (data->should_play_sel_hack) {
            data->should_play_sel_hack = false;
            if (ui::get_last_pl(1)) {
                pl::play_selected(ui::get_last_pl(0));
            }
        }
        ren::end_frame();
    }
    data->running = false;
}

void app::stop(bool rage) {
    data->rage_quit = rage;
    data->running = false;
}

void app::destroy() {
    data->running = false;
    if (data->stage > 3) {
        if (!data->rage_quit && data->should_save_conf)
            conf::save_to_file();
        pl::unload_playlists(data->rage_quit);
        audio::free_audio(audio::au);
        ui::destroy();
    }
    if (data->stage > 2)
        ren::destroy();
    win_handle = nullptr;
    if (data->stage > 1)
        SDL_DestroyWindow(data->win);
    if (data->stage > 0)
        SDL_Quit();
    logger::log_level = logger::ui_log_level = 0;
    Uint8* temp_bump = data->orig_bump;
    tf::bump_dl(data);
    mem::bump_ptr = nullptr;
    SDL_free(temp_bump);
}

void app::read_config() {
    // TODO: search config in other paths
    data->conf_path = data->data_path + "config.toml";
    size_t sz;
    const char* content = (const char*)SDL_LoadFile(data->conf_path.c_str(), &sz);
    if (!content)
        TF_WARN(<< "Failed to read config file (" << SDL_GetError() << ")");
    auto result = toml::try_parse_str(content ? content : "");
    if (result.is_err()) {
        auto errs = result.as_err();
        TF_ERROR(<< "Failed to parse config");
        for (auto it = errs.begin(); it != errs.end(); it++)
            TF_INFO(<< "Details - " << (*it).title());
    }
    if (result.is_ok()) {
        data->conf = result.as_ok();
        TF_INFO(<< "Config parsed successfully");
    }
    SDL_free((void*)content);
}

toml::value& conf::get() {
    return app::data->conf;
}

void conf::request_save() {
    app::data->should_save_conf = true;
}

bool conf::save_to_file() {
    auto data = toml::format(app::data->conf);
    if (!SDL_SaveFile(app::data->conf_path.c_str(), data.data(), data.size())) {
        TF_ERROR(<< "Failed to save config (" << SDL_GetError() << ")");
        return false;
    }
    return true;
}
