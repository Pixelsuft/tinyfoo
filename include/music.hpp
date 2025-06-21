#pragma once
#include <str.hpp>
#include <cstdint>

namespace audio {
    enum class Type {
        NONE = 0,
        WAV = 1,
        MOD = 2,
        MID = 3,
        OGG = 4,
        MP3 = 5,
        FLAC = 6,
        OPUS = 7,
        WAVPACK = 8,
        GME = 9
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
        uint64_t file_size;
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
            file_size = 0;
        }
    };
}
