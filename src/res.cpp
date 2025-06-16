#include <res.hpp>
#include <lbs.hpp>
#include <log.hpp>
#include <SDL3/SDL.h>
// TODO: optimize for release mode

SDL_IOStream* res::get_asset_io(const char* fn) {
    const char* path = nullptr;
    if (!SDL_strcmp(fn, "icon.png"))
        path = "assets/icon.png";
    else
        TF_ERROR(<< "Unknown asset: " << fn);
    SDL_IOStream* ret = SDL_IOFromFile(path, "rb");
    if (!ret) {
        TF_ERROR(<< "Failed to open io from asset file (" << SDL_GetError() << ")");
        return nullptr;
    }
    return ret;
}

void* res::read_asset_data(const char* fn, size_t& size_buf) {
    const char* path = nullptr;
    if (!SDL_strcmp(fn, "Roboto-Regular.ttf"))
        path = "assets/Roboto-Regular.ttf";
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
