#pragma once
#ifndef TOML11_DISABLE_STD_FILESYSTEM
#define TOML11_DISABLE_STD_FILESYSTEM
#endif
#include <toml.hpp>
#include <str.hpp>

namespace conf {
    toml::value& get();
    void request_save();
    bool save_to_file();
}
