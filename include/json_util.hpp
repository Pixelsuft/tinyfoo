#pragma once
#include <json.hpp>
#include <str.hpp>

namespace util {
    static inline tf::str json_unpack_str(const nlohmann::json& val) {
        // TODO: how???
        std::string ret = val;
        return tf::str(ret.c_str());
    }

    static inline const char* json_pack_str(const tf::str& s) {
        return s.c_str();
    }
}
