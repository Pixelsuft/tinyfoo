#include <app.hpp>
#include <ren.hpp>
#include <log.hpp>
#include <new.hpp>
#include <lbs.hpp>
#include <ui.hpp>
#include <audio_base.hpp>
#include <conf.hpp>
#include <SDL3/SDL.h>
#if IS_IMGUI
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#endif

namespace logger {
    extern int log_level;
}

namespace mem {
    extern Uint8* bump_ptr;
}

namespace app {
    void process_event(const SDL_Event& ev);

    struct AppData {
        toml::value conf;
        tf::str base_path;
        tf::str data_path;
        SDL_Window* win;
        Uint8* orig_bump;
        int stage;
        bool running;
    };

    AppData* data;

    void read_config();
}

bool app::init() {
    logger::log_level = 0;
    Uint8* temp_bump = mem::bump_ptr = (Uint8*)SDL_malloc(BUMP_SIZE);
    if (!mem::bump_ptr) {
        TF_FATAL(<< "Failed to create bump allocator");
        return false;
    }
    data = tf::bump_nw<AppData>();
    data->orig_bump = temp_bump;
    data->stage = 0;
    data->running = false;
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
    // This one actually never fails
    ui::init();
    data->stage = 4;
    // TODO
    audio::au = audio::create_sdl2_mixer(true);
    if (!audio::au->inited) {
        audio::free_audio(audio::au);
        audio::au = audio::create_sdl2_mixer(false);
    }
    if (!audio::au->inited) {
        audio::free_audio(audio::au);
        audio::au = audio::create_base();
        TF_WARN(<< "Using dummy audio library");
    }
    return true;
}

void app::process_event(const SDL_Event& ev) {
    ImGui_ImplSDL3_ProcessEvent(&ev);
    switch (ev.type) {
        case SDL_EVENT_QUIT: {
            data->running = false;
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            ui::update_size(ren::get_size());
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
        ren::begin_frame();
        ui::draw();
        ren::end_frame();
    }
    data->running = false;
}

void app::stop() {
    data->running = false;
}

void app::destroy() {
    data->running = false;
    if (data->stage > 3) {
        audio::free_audio(audio::au);
        ui::destroy();
    }
    if (data->stage > 2)
        ren::destroy();
    if (data->stage > 1)
        SDL_DestroyWindow(data->win);
    if (data->stage > 0)
        SDL_Quit();
    logger::log_level = 0;
    Uint8* temp_bump = data->orig_bump;
    tf::bump_dl(data);
    mem::bump_ptr = nullptr;
    SDL_free(temp_bump);
}

void app::read_config() {
    // TODO: search config in other paths
    tf::str fp = data->data_path + "config.toml";
    size_t sz;
    const char* content = (const char*)SDL_LoadFile(fp.c_str(), &sz);
    if (!content) {
        TF_WARN(<< "Failed to read config file (" << SDL_GetError() << ")");
        return;
    }
    auto result = toml::try_parse_str(content);
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
