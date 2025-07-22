#pragma once
#include <lbs.hpp>
#include <log.hpp>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_stdinc.h>

namespace rng {
    void reseed();
    int gen_int(int end);
}
