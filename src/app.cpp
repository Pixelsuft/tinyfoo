#include <app.hpp>
#include <ren.hpp>
#include <log.hpp>
#include <new.hpp>
#include <config.hpp>
#include <SDL3/SDL.h>
#include <ui.hpp>
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
        SDL_Window* win;
        Uint8* orig_bump;
        int stage;
        bool running;
    };

    AppData* data;
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
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        TF_FATAL(<< "Failed to init SDL (" << SDL_GetError() << ")");
        destroy();
        return false;
    }
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
    if (!SDL_SetWindowMinimumSize(data->win, 320, 200))
        TF_WARN(<< "Failed to set window minimum size (" << SDL_GetError() << ")");
    if (!ren::init(data->win)) {
        destroy();
        return false;
    }
    data->stage = 3;
    // This one actually never fails
    ui::init();
    data->stage = 4;
    return true;
}

void app::process_event(const SDL_Event& ev) {
    ImGui_ImplSDL3_ProcessEvent(&ev);
    switch (ev.type) {
        case SDL_EVENT_QUIT: {
            data->running = false;
            break;
        }
    }
}

void app::run() {
    SDL_Event ev;
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

void app::destroy() {
    data->running = false;
    if (data->stage > 3)
        ui::destroy();
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
