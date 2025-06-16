#include <res.hpp>
#include <lbs.hpp>
#include <log.hpp>
#include <SDL3/SDL.h>
// TODO: optimize for release mode

void* res::read_asset_data(const char* fn, size_t& size_buf) {
    const char* path = nullptr;
    if (!SDL_strcmp(fn, "Roboto-Regular.ttf"))
        path = "assets/Roboto-Regular.ttf";
    else if (!SDL_strcmp(fn, "icon.png"))
        path = "assets/icon.png";
    else
        TF_ERROR(<< "Unknown asset: " << fn);
    void* ret = SDL_LoadFile(path, &size_buf);
    if (!ret) {
        TF_ERROR(<< "Failed to read asset file (" << SDL_GetError() << ")");
        size_buf = 0;
        return nullptr;
    }
    return ret;
}

void res::free_asset_data(void* data) {
    SDL_free(data);
}
