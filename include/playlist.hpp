#pragma once
#include <str.hpp>
#include <vec.hpp>
#include <music.hpp>

namespace pl {
    struct Playlist {
        tf::vec<audio::Music*> mus;
        tf::str path;
        tf::str name;
        bool changed;
    };

    extern tf::vec<Playlist*>* pls;

    void load_playlists();
    void unload_playlists();
    void add_files_dialog(Playlist* p);
    void add_folder_dialog(Playlist* p);
    void save(Playlist* p);
}
