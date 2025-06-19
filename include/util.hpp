#pragma once
#include <json.hpp>
#include <str.hpp>
#include <lbs.hpp>
#include <ctime>

namespace util {
    static inline tf::str json_unpack_str(const nlohmann::json& val) {
        // TODO: how???
        std::string ret = val;
        return tf::str(ret.c_str());
    }

    static inline const char* json_pack_str(const tf::str& s) {
        return s.c_str();
    }

    static inline bool compare_paths(const tf::str& p1, const tf::str& p2) {
        // TODO: compare absolute and relative paths
#if IS_WIN && IS_RELEASE
        if (p1.size() != p2.size())
            return false;
        auto it2 = p2.begin();
        for (auto it = p1.begin(); it != p1.end(); it++) {
            if (((*it) == '\\' || (*it) == '//') && ((*it2) == '\\' || (*it2) == '//')) {
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
        static struct tm tm_buf = { 0 };
        time_t mod_time = (time_t)(sdl_time / 1000000000);
#if IS_MSVC
        struct tm* time_s = &tm_buf;
        if (localtime_s(time_s, &mod_time) != 0)
            tm_buf = { 0 };
#else
        struct tm* time_s = std::localtime(&mod_time);
        if (!time_s)
            time_s = &tm_buf;
#endif
        return time_s;
    }
}
