#pragma once
#ifndef TOML11_DISABLE_STD_FILESYSTEM
#define TOML11_DISABLE_STD_FILESYSTEM
#endif
#include <toml11.hpp>
#include <str.hpp>

namespace conf {
    toml::value& get();
    void request_save();
    bool save_to_file();

    static inline float read_float(const char* tab_name, const char* prop, float def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_integer())
            return (float)tab[prop].as_integer();
        if (tab[prop].is_floating())
            return (float)tab[prop].as_floating();
        return def_val;
    }

    static inline int read_int(const char* tab_name, const char* prop, int def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_integer())
            return (int)tab[prop].as_integer();
        return def_val;
    }

    static inline bool read_bool(const char* tab_name, const char* prop, bool def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_integer())
            return tab[prop].as_integer() > 0;
        if (tab[prop].is_boolean())
            return (bool)tab[prop].as_boolean();
        return def_val;
    }

    static inline tf::str read_str(const char* tab_name, const char* prop, const tf::str& def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_string())
            return toml::find<tf::str>(tab, prop);
        return def_val;
    }
}
