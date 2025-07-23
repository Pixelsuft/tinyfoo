#pragma once
#include <json.hpp>
#include <str.hpp>
#include <lbs.hpp>
#include <ctime>
#include <SDL3/SDL_stdinc.h>

namespace util {
    static inline tf::str json_unpack_str(const nlohmann::json& val) {
        // TODO: how???
        std::string ret = val;
        return tf::str(ret.c_str());
    }

    static inline const char* json_pack_str(const tf::str& s) {
        return s.c_str();
    }

    static inline bool str_starts_with(const tf::str& inp, const tf::str& mask) {
#if (defined(_MSVC_LANG) ? _MSVC_LANG : __cplusplus) >= 202002L
        return inp.starts_with(mask);
#else
        return inp.size() >= mask.size() && SDL_memcmp(&inp[0], &mask[0], mask.size()) == 0;
#endif
    }

    static inline bool str_matches_mask(const tf::str& inp, const tf::str& mask) {
        if (mask.size() > inp.size())
            return false;
        for (auto it = inp.begin(); it != inp.end() - mask.size() + 1; it++) {
            if (SDL_strncasecmp(&(*it), &mask[0], mask.size()) == 0)
                return true;
            /*
            bool can = true;
            for (int i = 0; i < (int)mask.size(); i++) {
                if (SDL_tolower(*(it + i)) != SDL_tolower(*(mask.begin() + i))) {
                    can = false;
                    break;
                }
            }
            if (can)
                return true;
            */
        }
        return false;
        // return inp.find(mask) != tf::str::npos;
    }

    static inline bool compare_paths(const tf::str& p1, const tf::str& p2) {
#if IS_WIN && IS_RELEASE
        if (p1.size() != p2.size())
            return false;
        auto it2 = p2.begin();
        for (auto it = p1.begin(); it != p1.end(); it++) {
            if (((*it) == '\\' || (*it) == '/') && ((*it2) == '\\' || (*it2) == '/')) {
                it2++;
                continue;
            }
            if (SDL_tolower(*it) == SDL_tolower(*it2)) {
                it2++;
                continue;
            }
            return false;
        }
        return true;
#else
        return p1 == p2;
#endif
    }

    static inline struct tm* tm_from_sdl_time(uint64_t sdl_time) {
        static struct tm tm_buf;
        time_t mod_time = (time_t)(sdl_time / 1000000000);
#if IS_MSVC
        struct tm* time_s = &tm_buf;
        if (localtime_s(time_s, &mod_time) != 0)
            SDL_zero(tm_buf);
#else
        struct tm* time_s = std::localtime(&mod_time);
        if (!time_s) {
            SDL_zero(tm_buf);
            time_s = &tm_buf;
        }
#endif
        return time_s;
    }
}
