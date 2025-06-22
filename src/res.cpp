#include <res.hpp>
#include <lbs.hpp>
#include <log.hpp>
#include <SDL3/SDL.h>
#if IS_RELEASE
#include <res_data.hpp>
#endif

void* res::get_asset_io(const char* fn) {
#if IS_RELEASE
    if (!SDL_strcmp(fn, "icon.png"))
        return SDL_IOFromConstMem((const void*)asset_icon, sizeof(asset_icon));
    return nullptr;
#else
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
#endif
}

void* res::read_asset_data(const char* fn, size_t& size_buf) {
#if IS_RELEASE
    if (!SDL_strcmp(fn, "Roboto-Regular.ttf")) {
        size_buf = sizeof(asset_font1);
        return (void*)asset_font1;
    }
    return nullptr;
#else
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
#endif
}

void res::free_asset_data(void* data) {
#if !IS_RELEASE
    SDL_free(data);
#endif
}
