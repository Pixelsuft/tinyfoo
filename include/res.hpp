#pragma once
#include <SDL3/SDL_iostream.h>

namespace res {
    SDL_IOStream* get_asset_io(const char* fn);
    void* read_asset_data(const char* fn, size_t& size_buf);
    void free_asset_data(void* data);
}
