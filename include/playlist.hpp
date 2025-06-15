#pragma once
#include <str.hpp>
#include <vec.hpp>
#include <music.hpp>

namespace pl {
    struct Playlist {
        tf::vec<audio::Music*> mus;
        tf::vec<int> selected;
        tf::str path;
        tf::str name;
        tf::str sorting;
        int last_sel;
        bool changed;
    };

    extern tf::vec<Playlist*>* pls;

    tf::str full_path_for_playlist(const tf::str& path);
    void load_playlists();
    void unload_playlists();
    void add_files_dialog(Playlist* p);
    void add_folder_dialog(Playlist* p);
    bool save(Playlist* p);
    void sort_by(Playlist* p, const char* what);
}
