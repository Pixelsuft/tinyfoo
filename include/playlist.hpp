#pragma once
#include <str.hpp>
#include <vec.hpp>
#include <music.hpp>

namespace pl {
    struct Playlist {
        tf::vec<audio::Music*> mus;
        tf::vec<audio::Music*> remembering;
        tf::vec<int> selected;
        tf::str path;
        tf::str name;
        tf::str sorting;
        int last_shift_sel;
        int last_shift_sel2;
        int last_sel;
        bool changed;
        bool reserve_sorting;
    };

    extern tf::vec<Playlist*>* pls;

    tf::str full_path_for_playlist(const tf::str& path);
    void load_playlists();
    void unload_playlists();
    void add_file_by_fp(Playlist*, const char* fp);
    void add_files_dialog(Playlist* p);
    void add_folder_dialog(Playlist* p);
    void clear_selected(Playlist* p);
    void remove_selected(Playlist* p);
    void play_selected(Playlist* p);
    void remember_selected(Playlist* p);
    void unremember_selected(Playlist* p);
    void select_all(Playlist* p);
    bool save(Playlist* p);
    void sort_by(Playlist* p, const char* what);
}
