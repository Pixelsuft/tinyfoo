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
#if ENABLE_IMGUI
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
#if ENABLE_TOMLPP
        toml::table conf;
#else
        toml::value conf;
#endif
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
#ifdef ENABLE_IMGUI
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
    auto backend_au = conf::read_str("audio", "backend", "");
    if (0) {}
#if ENABLE_SDL2_MIXER
    else if (backend_au == "sdl2_mixer_ext")
        audio::au = audio::create_sdl2_mixer(true);
    else if (backend_au == "sdl2_mixer")
        audio::au = audio::create_sdl2_mixer(false);
#endif
#if ENABLE_FMOD
    else if (backend_au == "fmod")
        audio::au = audio::create_fmod();
#endif
#if ENABLE_BASS
    else if (backend_au == "bass")
        audio::au = audio::create_bass();
#endif
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
#ifdef ENABLE_IMGUI
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
#if ENABLE_TOMLPP
    toml::parse_result result = toml::parse(content ? content : "");
    if (!result) {
        TF_ERROR(<< "Failed to parse config (" << result.error() << ")");
        result = toml::parse(""); // Hopefully this never fails
    }
    data->conf = std::move(result.table());
#else
    auto result = toml::try_parse_str(content ? content : "");
    if (result.is_err()) {
        auto& errs = result.as_err();
        TF_ERROR(<< "Failed to parse config");
        for (auto it = errs.begin(); it != errs.end(); it++)
            TF_INFO(<< "Details - " << (*it).title());
        result = toml::try_parse_str(""); // Also this
    }
    if (result.is_ok()) {
        data->conf = result.as_ok();
        TF_INFO(<< "Config parsed successfully");
    }
#endif
    SDL_free((void*)content);
}

conf::toml_table& conf::get() {
    return app::data->conf;
}

void conf::request_save() {
    app::data->should_save_conf = true;
}

bool conf::save_to_file() {
#if ENABLE_TOMLPP
    std::stringstream stream;
    stream << app::data->conf;
    auto data = stream.str();
#else
    auto data = toml::format(app::data->conf);
#endif
    if (!SDL_SaveFile(app::data->conf_path.c_str(), data.data(), data.size())) {
        TF_ERROR(<< "Failed to save config (" << SDL_GetError() << ")");
        return false;
    }
    return true;
}

void conf::begin_editing(ConfData& data) {
    data.dev_names.clear();
    audio::au->dev_fill_arr(data.dev_names);
    auto need_it = std::find(data.dev_names.begin(), data.dev_names.end(), audio::au->need_dev);
    if (need_it == data.dev_names.end())
        data.ints[4] = 0;
    else
        data.ints[4] = (int)std::distance(data.dev_names.begin(), need_it);
    data.floats[3] = audio::au->volume * 100.f;
    data.floats[4] = audio::au->fade_next_time * 1000.f;
    data.floats[5] = audio::au->fade_stop_time * 1000.f;
    data.floats[6] = audio::au->fade_pause_time * 1000.f;
    data.floats[7] = audio::au->fade_resume_time * 1000.f;
    data.style = data.def_style = conf::read_str("imgui", "style", "dark");
    data.font1_path = conf::read_str("imgui", "font1_path", "");
    data.font2_path = conf::read_str("imgui", "font2_path", "");
    data.floats[0] = conf::read_float("imgui", "font1_size", 16.f);
    data.floats[1] = conf::read_float("imgui", "font2_size", 24.f);
    data.floats[2] = conf::read_float("imgui", "img_scale", 1.f);
    data.ren_drv = conf::read_str("renderer", "driver", "auto");
    data.bools[0] = conf::read_bool("renderer", "vsync", true);
    data.au_bk = conf::read_str("audio", "backend", "dummy");
    data.sdl2_drv = conf::read_str("sdl2_mixer", "driver", "dummy");
    data.sdl2_fmt = conf::read_str("sdl2_mixer", "format", "SDL_AUDIO_S16");
    data.bools[1] = conf::read_bool("sdl2_mixer", "enable_flac", false);
    data.bools[2] = conf::read_bool("sdl2_mixer", "enable_mod", false);
    data.bools[3] = conf::read_bool("sdl2_mixer", "enable_mp3", true);
    data.bools[4] = conf::read_bool("sdl2_mixer", "enable_ogg", false);
    data.bools[5] = conf::read_bool("sdl2_mixer", "enable_mid", false);
    data.bools[6] = conf::read_bool("sdl2_mixer", "enable_opus", false);
    data.bools[7] = conf::read_bool("sdl2_mixer", "enable_wavpack", false);
    data.ints[0] = conf::read_int("sdl2_mixer", "channels", 0);
    data.ints[1] = conf::read_int("sdl2_mixer", "frequency", 0);
    data.ints[2] = conf::read_int("sdl2_mixer", "chunksize", 0);
    data.fmod_drv = conf::read_str("fmod", "driver", "nosound");
    data.bools[8] = conf::read_bool("bass", "force_16bits", false);
    data.bools[9] = conf::read_bool("bass", "force_stereo", false);
    data.bools[10] = conf::read_bool("bass", "force_dmix", false);
    data.bools[11] = conf::read_bool("bass", "force_audiotrack", false);
    data.bools[12] = conf::read_bool("bass", "force_directsound", false);
    data.bools[13] = conf::read_bool("bass", "force_software", false);
    data.ints[3] = conf::read_int("bass", "frequency", 0);
}

#if ENABLE_TOMLPP
// HELP
#define TOML_DUMP_STR(str) str.c_str()
#else
#define TOML_DUMP_STR(str) str
#endif
void conf::end_editing(ConfData& data) {
    data.def_style = data.style;
    audio::au->need_dev = data.dev_names[data.ints[4]];
    audio::au->volume = data.floats[3] / 100.f;
    audio::au->update_volume();
    audio::au->fade_next_time = data.floats[4] / 1000.f;
    audio::au->fade_stop_time = data.floats[5] / 1000.f;
    audio::au->fade_pause_time = data.floats[6] / 1000.f;
    audio::au->fade_resume_time = data.floats[7] / 1000.f;
    // TODO: support tab order saving, but IDK how to do that easily
#if ENABLE_TOMLPP
    toml::array pl_files;
    pl_files.reserve(pl::pls->size());
    for (auto it = pl::pls->begin(); it != pl::pls->end(); it++)
        pl_files.push_back((*it)->path.c_str());
#else
    tf::vec<tf::str> pl_files;
    pl_files.reserve(pl::pls->size());
    for (auto it = pl::pls->begin(); it != pl::pls->end(); it++)
        pl_files.push_back((*it)->path);
#endif
    app::data->conf = toml::table{
        {"renderer", toml::table{
            {"driver", TOML_DUMP_STR(data.ren_drv)},
            {"vsync", data.bools[0]}
        }},
        {"imgui", toml::table{
            {"style", TOML_DUMP_STR(data.style)},
            {"img_scale", data.floats[2]},
            {"font1_path", TOML_DUMP_STR(data.font1_path)},
            {"font1_size", data.floats[0]},
            {"font2_path", TOML_DUMP_STR(data.font2_path)},
            {"font2_size", data.floats[1]}
        }},
        {"audio", toml::table{
            {"backend", TOML_DUMP_STR(data.au_bk)},
            {"device", TOML_DUMP_STR(audio::au->need_dev)},
            {"volume", data.floats[3]},
            {"fade_next_time", data.floats[4]},
            {"fade_stop_time", data.floats[5]},
            {"fade_pause_time", data.floats[6]},
            {"fade_resume_time", data.floats[7]}
        }},
        {"sdl2_mixer", toml::table{
            {"driver", TOML_DUMP_STR(data.sdl2_drv)},
            {"format", TOML_DUMP_STR(data.sdl2_fmt)},
            {"channels", data.ints[0]},
            {"frequency", data.ints[1]},
            {"chunksize", data.ints[2]},
            {"enable_flac", data.bools[1]},
            {"enable_mod", data.bools[2]},
            {"enable_mp3", data.bools[3]},
            {"enable_ogg", data.bools[4]},
            {"enable_mid", data.bools[5]},
            {"enable_opus", data.bools[6]},
            {"enable_wavpack", data.bools[7]}
        }},
        {"fmod", toml::table{
            {"driver", TOML_DUMP_STR(data.fmod_drv)}
        }},
        {"bass", toml::table{
            {"frequency", data.ints[3]},
            {"force_16bits", data.bools[8]},
            {"force_stereo", data.bools[9]},
            {"force_dmix", data.bools[10]},
            {"force_audiotrack", data.bools[11]},
            {"force_directsound", data.bools[12]},
            {"force_software", data.bools[13]}
        }},
        {"playlists", toml::table{
            {"files", pl_files}
        }}
    };
    conf::request_save();
}
