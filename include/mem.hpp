#pragma once
#include <lbs.hpp>
#if IS_RELEASE
#include <SDL3/SDL_stdinc.h>
#endif

namespace mem {
#if IS_RELEASE
    static inline void* alloc(int size) {
        return SDL_malloc((size_t)size);
    }

    static inline void free(void* ptr) {
        SDL_free(ptr);
    }
#else
    void* alloc(int size);
    void free(void* ptr);
#endif
    void* bump_alloc(int size);
}
