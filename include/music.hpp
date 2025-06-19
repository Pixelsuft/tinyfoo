#pragma once
#include <str.hpp>

namespace audio {
    enum class Type {
        NONE,
        WAV,
        MOD,
        MID,
        OGG,
        MP3,
        FLAC,
        OPUS,
        WAVPACK,
        GME
    };

    static const char* get_type_str(Type tp) {
        switch (tp) {
            case Type::NONE: {
                return "NONE";
            }
            case Type::WAV: {
                return "WAV";
            }
            case Type::MOD: {
                return "MOD";
            }
            case Type::MID: {
                return "MID";
            }
            case Type::OGG: {
                return "OGG";
            }
            case Type::MP3: {
                return "MP3";
            }
            case Type::FLAC: {
                return "FLAC";
            }
            case Type::OPUS: {
                return "OPUS";
            }
            case Type::WAVPACK: {
                return "WAVPACK";
            }
            case Type::GME: {
                return "GME";
            }
        }
        return "NONE";
    }

    struct Music {
        tf::str full_path;
        tf::str fn;
        uint64_t last_mod;
        uint64_t last_click;
        void* h1;
        void* h2;
        float dur;
        Type type;
        bool selected;

        inline Music() {
            h1 = nullptr;
            h2 = nullptr;
            dur = -1.f;
            type = Type::NONE;
            selected = false;
            last_click = 0;
            last_mod = 0;
        }
    };
}
