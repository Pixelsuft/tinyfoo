#pragma once
#include <lbs.hpp>
#include <str.hpp>
#include <vec.hpp>
#if ENABLE_TOMLPP
#include <string_view>
#ifndef TOML_EXCEPTIONS
#define TOML_EXCEPTIONS 0
#endif
#include <tomlpp.hpp>
#else
#ifndef TOML11_DISABLE_STD_FILESYSTEM
#define TOML11_DISABLE_STD_FILESYSTEM
#endif
#include <toml11.hpp>
#endif

namespace conf {
#if ENABLE_TOMLPP
    using toml_table = toml::table;
#else
    using toml_table = toml::value;
#endif
    static const char* ren_drv[] = { "auto", "direct3d", "direct3d11", "direct3d12", "opengl", "opengles", "opengles2", "vulkan", "gpu", "software" };
    static const char* ig_style_list[] = {
        "dark", "light", "classic", "adobe", "cherry", "darky", "deep_dark", "discord",
        "dracula", "duck_red", "enemymouse", "gold", "gpulib", "green_font",
        "material_flat", "mediacy", "photoshop", "red_font", "ruda", "sonic_riders", "ue4",
        "vgui", "visual_studio", "windark"
    };
    static const char* au_bk[] = { "dummy", "sdl2_mixer", "sdl2_mixer_ext", "fmod", "bass" };
    static const char* sdl2_drv[] = { "default", "aaudio", "alsa", "coreaudio", "directsound", "disk", "dummy", "emscripten", "haiku", "jack", "n3ds", "netbsd", "ngage", "opensles", "oss", "pipewire", "ps2", "psp", "pulseaudio", "qnx", "sndio", "vita", "wasapi" };
    static const char* sdl2_fmt[] = { "SDL_AUDIO_UNKNOWN", "SDL_AUDIO_U8", "SDL_AUDIO_S8", "SDL_AUDIO_S16LE", "SDL_AUDIO_S16BE", "SDL_AUDIO_S32LE", "SDL_AUDIO_S32BE", "SDL_AUDIO_F32LE", "SDL_AUDIO_F32BE", "SDL_AUDIO_S16", "SDL_AUDIO_S32", "SDL_AUDIO_F32" };
    static const char* fmod_drv[] = { "default", "nosound", "wavwriter", "nosound_nrt", "wavwriter_nrt", "wasapi", "asio", "pulseaudio", "alsa", "coreaudio", "audiotrack", "opensl", "audioout", "audio3d", "webaudio", "nnaudio", "winsonic", "aaudio", "audioworklet", "phase", "ohaudio" };

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
        int ints[8];
    };

    void begin_editing(ConfData& data);
    void end_editing(ConfData& data);
    toml_table& get();
    void request_save();
    bool save_to_file();

    static inline float read_float(const char* tab_name, const char* prop, float def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
#if ENABLE_TOMLPP
        auto& tab = conf::get()[tab_name].ref<toml::table>();
        if (tab[prop].is_integer())
            return (float)*tab[prop].value<int>();
        if (tab[prop].is_floating_point())
            return *tab[prop].value<float>();
        return def_val;
#else
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_integer())
            return (float)tab[prop].as_integer();
        if (tab[prop].is_floating())
            return (float)tab[prop].as_floating();
        return def_val;
#endif
    }

    static inline int read_int(const char* tab_name, const char* prop, int def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
#if ENABLE_TOMLPP
        auto& tab = conf::get()[tab_name].ref<toml::table>();
        if (tab[prop].is_integer())
            return *tab[prop].value<int>();
        return def_val;
#else
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_integer())
            return (int)tab[prop].as_integer();
        return def_val;
#endif
    }

    static inline bool read_bool(const char* tab_name, const char* prop, bool def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
#if ENABLE_TOMLPP
        auto& tab = conf::get()[tab_name].ref<toml::table>();
        if (tab[prop].is_integer())
            return *tab[prop].value<int>() > 0;
        if (tab[prop].is_boolean())
            return *tab[prop].value<bool>();
        return def_val;
#else
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_integer())
            return tab[prop].as_integer() > 0;
        if (tab[prop].is_boolean())
            return (bool)tab[prop].as_boolean();
        return def_val;
#endif
    }

    static inline tf::str read_str(const char* tab_name, const char* prop, const tf::str& def_val) {
        if (!conf::get().contains(tab_name) || !conf::get()[tab_name].is_table())
            return def_val;
#if ENABLE_TOMLPP
        auto& tab = conf::get()[tab_name].ref<toml::table>();
        if (tab[prop].is_string())
            return tf::str(*tab[prop].value<std::string_view>());
        return def_val;
#else
        toml::value tab = conf::get()[tab_name];
        if (tab[prop].is_string())
            return toml::find<tf::str>(tab, prop);
        return def_val;
#endif
    }
}
