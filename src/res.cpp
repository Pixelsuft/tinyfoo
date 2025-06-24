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
    else if (!SDL_strcmp(fn, "icon_stop.png"))
        path = "assets/icon_stop.png";
    else if (!SDL_strcmp(fn, "icon_play.png"))
        path = "assets/icon_play.png";
    else if (!SDL_strcmp(fn, "icon_pause.png"))
        path = "assets/icon_pause.png";
    else if (!SDL_strcmp(fn, "icon_back.png"))
        path = "assets/icon_back.png";
    else if (!SDL_strcmp(fn, "icon_fwd.png"))
        path = "assets/icon_fwd.png";
    else if (!SDL_strcmp(fn, "icon_rng.png"))
        path = "assets/icon_rng.png";
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

void* res::read_asset_data(const char* fn, int& size_buf) {
#if IS_RELEASE
    if (!SDL_strcmp(fn, "Roboto-Regular.ttf")) {
        size_buf = (int)sizeof(asset_font1);
        return (void*)asset_font1;
    }
    return nullptr;
#else
    const char* path = nullptr;
    if (!SDL_strcmp(fn, "Roboto-Regular.ttf"))
        path = "assets/Roboto-Regular.ttf";
    else
        TF_ERROR(<< "Unknown asset: " << fn);
    size_t load_size_buf;
    void* ret = SDL_LoadFile(path, &load_size_buf);
    if (!ret) {
        TF_ERROR(<< "Failed to read asset file (" << SDL_GetError() << ")");
        size_buf = 0;
        return nullptr;
    }
    size_buf = (int)load_size_buf;
    return ret;
#endif
}

void res::free_asset_data(void* data) {
#if !IS_RELEASE
    SDL_free(data);
#endif
}
