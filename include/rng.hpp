#pragma once
#include <lbs.hpp>
#include <log.hpp>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_stdinc.h>

namespace rng {
    static inline void reseed() {
        SDL_Time ticks;
        if (SDL_GetCurrentTime(&ticks)) {
            SDL_srand((Uint64)ticks);
        }
        else {
            TF_WARN(<< "Failed to get current time (" << SDL_GetError() << ")");
            SDL_srand(0);
        }
    }

    template<typename T>
    static inline int gen_int(T end) {
        return (int)SDL_rand((Sint32)end);
    }
}
