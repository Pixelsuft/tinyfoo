#pragma once
#include <lbs.hpp>
#include <str.hpp>
#include <vec.hpp>
#include <music.hpp>
#if ORDER_COOL_RNG_PATCH
#include <map.hpp>
#endif

namespace pl {
    struct Playlist {
#if ORDER_COOL_RNG_PATCH
        tf::map<tf::str, tf::vec<audio::Music*>> artist_map;
#endif
        tf::vec<audio::Music*> mus;
        tf::vec<audio::Music*> repeating;
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
    void unload_playlists(bool rage);
    void add_new_pl();
    void remove_pl(Playlist* p);
    void remove_dead(Playlist* p);
    void scan_changes(Playlist* p);
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
    bool mus_open_file(audio::Music* mus);
    void fill_cache();
    void reload_cache(int mode);
    void update_cache();
}
