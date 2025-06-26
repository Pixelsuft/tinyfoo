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
        GME = 9,
        PXTONE = 10,
        AAC = 11,
        AIFF = 12,
        ASF = 13,
        DLS = 14,
        FSB = 15,
        IT = 16,
        RAW = 17,
        S3M = 18,
        XM = 19,
        VORBIS = 20,
        AUDIOQUEUE = 21,
        MFA = 22,
        M4A = 23,
        FADPCM = 24,
        WMA = 25,
        MP1 = 26,
        MP2 = 27,
        MTM = 28,
        MO3 = 29
    };

    static const char* get_type_str(Type tp) {
        switch (tp) {
            case Type::NONE: {
                return "???";
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
            case Type::MP1: {
                return "MP1";
            }
            case Type::MP2: {
                return "MP2";
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
            case Type::PXTONE: {
                return "PXTONE";
            }
            case Type::AAC: {
                return "AAC";
            }
            case Type::AIFF: {
                return "AIFF";
            }
            case Type::ASF: {
                return "ASF";
            }
            case Type::FSB: {
                return "FSB";
            }
            case Type::IT: {
                return "IT";
            }
            case Type::RAW: {
                return "RAW";
            }
            case Type::S3M: {
                return "S3M";
            }
            case Type::XM: {
                return "XM";
            }
            case Type::VORBIS: {
                return "VORBIS";
            }
            case Type::AUDIOQUEUE: {
                return "AUDIOQUEUE";
            }
            case Type::MFA: {
                return "MFA";
            }
            case Type::M4A: {
                return "M4A";
            }
            case Type::FADPCM: {
                return "FADPCM";
            }
            case Type::WMA: {
                return "WMA";
            }
            case Type::MTM: {
                return "MTM";
            }
            case Type::MO3: {
                return "MO3";
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
        bool cached;
        bool selected;

        inline Music() {
            h1 = nullptr;
            h2 = nullptr;
            dur = -1.f;
            type = Type::NONE;
            selected = false;
            cached = false;
            last_click = 0;
            last_mod = 0;
            file_size = 0;
        }
    };
}
