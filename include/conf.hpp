#pragma once
#ifndef TOML11_DISABLE_STD_FILESYSTEM
#define TOML11_DISABLE_STD_FILESYSTEM
#endif
#include <toml.hpp>
#include <str.hpp>

namespace conf {
    toml::value& get();
    void request_save();
    bool save_to_file();

    static inline float read_float(toml::value& tab, const char* prop, float def_val) {
        if (tab[prop].is_integer())
            return (float)tab[prop].as_integer();
        if (tab[prop].is_floating())
            return (float)tab[prop].as_floating();
        return def_val;
    }
}
