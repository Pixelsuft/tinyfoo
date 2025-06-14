#pragma once
#include <str.hpp>
#include <vec.hpp>
#include <music.hpp>

namespace pl {
    struct Playlist {
        tf::str path;
        tf::str name;
        bool changed;
    };

    extern tf::vec<Playlist*>* pls;

    void load_playlists();
    void unload_playlists();
}
