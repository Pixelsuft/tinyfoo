#pragma once
#include <lbs.hpp>
#include <str.hpp>
#include <vec.hpp>
#ifndef TOML11_DISABLE_STD_FILESYSTEM
#define TOML11_DISABLE_STD_FILESYSTEM
#endif
#include <toml11.hpp>

namespace conf {
    struct ConfData {
        tf::vec<tf::str> dev_names;
        tf::str style;
        tf::str def_style;
        tf::str font1_path;
        tf::str font2_path;
        tf::str sdl2_drv;
        tf::str sdl2_fmt;
        tf::str fmod_drv;
        tf::str ren_drv;
        tf::str au_bk;
        bool bools[16];
        float floats[8];
        int ints[4];
        int dev_id;
    };

    void begin_editing(ConfData& data);
    void end_editing(ConfData& data);
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
