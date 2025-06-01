#include <ren.hpp>
#include <new.hpp>
#include <log.hpp>
#include <SDL3/SDL.h>

namespace ren {
    struct RenData {
        SDL_Renderer* ren;
    };

    RenData* data;
}

bool ren::init(void* win) {
    data = tf::bump_nw<RenData>();
    // Should I handle props errors?
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, win);
    SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, "direct3d"); // TODO
    SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
    data->ren = SDL_CreateRendererWithProperties(props);
    if (!data->ren) {
        TF_FATAL(<< "Failed to create SDL renderer (" << SDL_GetError() << ")");
        SDL_DestroyProperties(props);
        tf::bump_dl(data);
        return false;
    }
    SDL_DestroyProperties(props);
    return true;
}

void ren::destroy() {
    tf::bump_dl(data);
}
