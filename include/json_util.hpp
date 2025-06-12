#pragma once
#include <json.hpp>
#include <str.hpp>

namespace util {
    static inline tf::str json_str(const nlohmann::json& val) {
        // TODO: how???
        std::string ret = val;
        return tf::str(ret.c_str());
    }
}
