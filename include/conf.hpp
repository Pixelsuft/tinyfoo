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

    static inline int read_int(toml::value& tab, const char* prop, int def_val) {
        if (tab[prop].is_integer())
            return (int)tab[prop].as_integer();
        return def_val;
    }

    static inline bool read_bool(toml::value& tab, const char* prop, bool def_val) {
        if (tab[prop].is_boolean())
            return (bool)tab[prop].as_boolean();
        return def_val;
    }

    static inline tf::str read_str(toml::value& tab, const char* prop, const tf::str& def_val) {
        if (tab[prop].is_string())
            return toml::find<tf::str>(tab, prop);
        return def_val;
    }
}
