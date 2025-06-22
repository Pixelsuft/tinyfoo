#include <playlist.hpp>
#include <log.hpp>
#include <unreachable.hpp>
#include <new.hpp>
#include <lbs.hpp>
#include <audio_base.hpp>
#include <conf.hpp>
#include <json.hpp>
#include <util.hpp>
#include <algorithm>
#include <SDL3/SDL.h>
#if IS_WIN
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

using nlohmann::json;

namespace app {
    extern void* win_handle;

    tf::str get_data_path();
}

namespace ui {
    void update_meta_info();
    void fix_selected_pl();
    pl::Playlist* get_last_pl(int hacky);
}

namespace pl {
    tf::vec<Playlist*>* pls;

    bool load_pl_from_fp(const tf::str& fp);
    void audio_clear_cache();
    void file_mod_time(const char* path, uint64_t& mod_t_buf, uint64_t& size_buf);
    void check_music_mod(audio::Music* mus);
    void fill_cache();
}

static inline tf::str fn_from_fp(const tf::str& fp) {
    tf::str ret = fp;
    auto t1_find = fp.rfind('\\') + 1;
    auto t2_find = fp.rfind('/') + 1;
    if (t1_find >= 0 && t1_find < fp.size())
        ret = fp.substr(t1_find);
    else if (t2_find >= 0 && t2_find < fp.size())
        ret = fp.substr(t2_find);
    else
        ret = fp;
    t1_find = ret.rfind('.');
    if (t1_find >= 0 && t1_find < ret.size())
        ret = ret.substr(0, t1_find);
    return ret;
}

void pl::file_mod_time(const char* path, uint64_t& mod_t_buf, uint64_t& size_buf) {
    mod_t_buf = size_buf = 0;
    SDL_PathInfo info;
    if (!SDL_GetPathInfo(path, &info)) {
        TF_WARN(<< "Failed to get file info (" << SDL_GetError() << ")");
        return;
    }
    if (info.type == SDL_PATHTYPE_FILE) {
        mod_t_buf = info.modify_time;
        size_buf = info.size;
    }
}

tf::str pl::full_path_for_playlist(const tf::str& path) {
    if ((path.find('/') < 0 || path.find('/') >= path.size()) && (path.find('\\') < 0 || path.find('\\') >= path.size()))
        return app::get_data_path() + path;
    else
        return path;
}

bool pl::load_pl_from_fp(const tf::str& fp) {
    TF_INFO(<< "Loading playlist " << fp);
    size_t sz;
    const char* data = (const char*)SDL_LoadFile(fp.c_str(), &sz);
    if (!data) {
        TF_ERROR(<< "Failed to load playlist file (" << SDL_GetError() << ")");
        return false;
    }
    json d = json::parse(data);
    SDL_free((void*)data);
    if (!d.is_object() || !d["name"].is_string()) {
        TF_ERROR(<< "Failed to parse playlist");
        return false;
    }
    pl::Playlist* p = tf::nw<pl::Playlist>();
    // p->path = fp;
    p->name = util::json_unpack_str(d["name"]);
    if (d["sort"].is_string())
        p->sorting = util::json_unpack_str(d["sort"]);
    else
        p->sorting = "none";
    if (d["sort_rev"].is_boolean())
        p->reserve_sorting = d["sort_rev"];
    else
        p->reserve_sorting = false;
    p->last_sel = p->last_shift_sel = p->last_shift_sel2 = 0;
    p->changed = false;
    if (d["content"].is_array()) {
        auto arr = d["content"].items();
        p->mus.reserve(d["content"].size());
        for (auto it = arr.begin(); it != arr.end(); it++) {
            auto val = (*it).value();
            if (!val.is_object() || !val["fp"].is_string())
                continue;
            audio::Music* m = tf::nw<audio::Music>();
            m->full_path = util::json_unpack_str(val["fp"]);
            m->fn = fn_from_fp(m->full_path);
            m->last_mod = (val["mod"].is_number_integer() && val["mod"].is_number_unsigned()) ? (uint64_t)val["mod"] : 0;
            m->file_size = (val["sz"].is_number_integer() && val["sz"].is_number_unsigned()) ? (uint64_t)val["sz"] : 0;
            m->dur = val["dur"].is_number_float() ? (float)val["dur"] : 0.f;
            m->type = (val["tp"].is_number_integer() && val["tp"].is_number_unsigned()) ? (audio::Type)((int)val["tp"]) : audio::Type::NONE;
            // TODO: config var to validate music changes on start
            // check_music_mod(m);
            p->mus.push_back(m);
        }
    }
    sort_by(p, p->sorting.c_str());
    if (p->reserve_sorting)
        sort_by(p, "reverse");
    pl::pls->push_back(p);
    return true;
}

void pl::load_playlists() {
    if (!conf::get().contains("playlists") || !conf::get().at("playlists").is_table())
        return;
    toml::value tab = conf::get().at("playlists");
    if (!tab.contains("files") || !tab.at("files").is_array())
        return;
    toml::array arr = tab.at("files").as_array();
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr.at(i).is_string()) {
            tf::str file_name = tf::str(arr.at(i).as_string().c_str());
            // TODO: has it / or \\?
            if (load_pl_from_fp(full_path_for_playlist(file_name))) {
                pl::pls->at(pl::pls->size() - 1)->path = file_name;
            }
        }
    }
}

bool pl::mus_open_file(audio::Music* mus) {
    // TODO: support custom handles like SDL_IOstream maybe? (currently not because of sdl2-compat)
    return audio::au->mus_open_fp(mus, mus->full_path.c_str());
}

void pl::add_file_by_fp(Playlist* p, const char* fp) {
    // TF_INFO(<< "Adding file " << fp);
    audio::Music* m = tf::nw<audio::Music>();
    m->full_path = fp;
    m->h1 = m->h2 = nullptr;
    for (auto it = p->mus.begin(); it != p->mus.end(); it++) {
        if (util::compare_paths((*it)->full_path, m->full_path)) {
            TF_WARN(<< "File already in playlist (" << fp << ")");
            tf::dl(m);
            return;
        }
    }
    file_mod_time(fp, m->last_mod, m->file_size);
    if (!mus_open_file(m)) {
        TF_WARN(<< "Failed to add file " << fp);
        tf::dl(m);
        return;
    }
    m->fn = fn_from_fp(m->full_path);
    audio::au->mus_fill_info(m);
    audio::au->mus_close(m);
    p->mus.push_back(m);
    p->changed = true;
}

void SDLCALL pl_files_cb(void* data, const char* const* filelist, int filter) {
    (void)filter;
    if (!filelist)
        return;
    const char* const* ptr = filelist;
    while (*ptr) {
        pl::add_file_by_fp((pl::Playlist*)data, *ptr);
        ptr++;
    }
}

void pl::add_files_dialog(Playlist* p) {
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_FILE_DIALOG_WINDOW_POINTER, app::win_handle);
    SDL_SetBooleanProperty(props, SDL_PROP_FILE_DIALOG_MANY_BOOLEAN, true);
    SDL_SetStringProperty(props, SDL_PROP_FILE_DIALOG_TITLE_STRING, "Add files...");
    SDL_ShowFileDialogWithProperties(SDL_FILEDIALOG_OPENFILE, pl_files_cb, (void*)p, props);
    SDL_DestroyProperties(props);
}

void SDLCALL pl_folder_cb(void* data, const char* const* filelist, int filter) {
    (void)filter;
    if (!filelist)
        return;
    const char* const* ptr = filelist;
    while (*ptr) {
        int count;
        char** content = SDL_GlobDirectory(*ptr, nullptr, SDL_GLOB_CASEINSENSITIVE, &count);
        if (content) {
            ((pl::Playlist*)data)->mus.reserve(((pl::Playlist*)data)->mus.size() + (size_t)count);
            tf::str dir_path(*ptr);
            for (int i = 0; i < count; i++) {
                pl::add_file_by_fp((pl::Playlist*)data, (dir_path + PATH_SEP + content[i]).c_str());
            }
            SDL_free(content);
        }
        else {
            TF_ERROR(<< "Failed to glob directory (" << SDL_GetError() << ")");
        }
        ptr++;
    }
}

void pl::add_folder_dialog(Playlist* p) {
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_FILE_DIALOG_WINDOW_POINTER, app::win_handle);
    SDL_SetBooleanProperty(props, SDL_PROP_FILE_DIALOG_MANY_BOOLEAN, false);
    SDL_SetStringProperty(props, SDL_PROP_FILE_DIALOG_TITLE_STRING, "Add folder...");
    SDL_ShowFileDialogWithProperties(SDL_FILEDIALOG_OPENFOLDER, pl_folder_cb, (void*)p, props);
    SDL_DestroyProperties(props);
}

int SDLCALL mus_compare_by_name(const audio::Music** a, const audio::Music** b) {
    if ((*a)->fn > (*b)->fn)
        return 1;
    if ((*a)->fn < (*b)->fn)
        return -1;
    return 0;
}

int SDLCALL mus_compare_by_dur(const audio::Music** a, const audio::Music** b) {
    if ((*a)->dur > (*b)->dur)
        return 1;
    if ((*a)->dur < (*b)->dur)
        return -1;
    return 0;
}

int SDLCALL mus_compare_by_mod_time(const audio::Music** a, const audio::Music** b) {
    if ((*a)->last_mod > (*b)->last_mod)
        return 1;
    if ((*a)->last_mod < (*b)->last_mod)
        return -1;
    return 0;
}

void pl::sort_by(Playlist* p, const char* what) {
    if (SDL_strlen(what) == 0 || !SDL_strcmp(what, "none"))
        return;
    if (!SDL_strcmp(what, "fn"))
        SDL_qsort(p->mus.data(), p->mus.size(), sizeof(audio::Music*), (SDL_CompareCallback)mus_compare_by_name);
    else if (!SDL_strcmp(what, "dur"))
        SDL_qsort(p->mus.data(), p->mus.size(), sizeof(audio::Music*), (SDL_CompareCallback)mus_compare_by_dur);
    else if (!SDL_strcmp(what, "mod_time"))
        SDL_qsort(p->mus.data(), p->mus.size(), sizeof(audio::Music*), (SDL_CompareCallback)mus_compare_by_mod_time);
    else if (!SDL_strcmp(what, "reverse"))
        std::reverse(p->mus.begin(), p->mus.end());
    else
        TF_ERROR(<< "Unkown playlist sort by (" << what << ")");
}

static inline bool write_file(const char* fp, const char* data, size_t size) {
    if (!SDL_SaveFile(fp, (const void*)data, size)) {
        TF_ERROR(<< "Failed to save file (" << SDL_GetError() << ")");
        return false;
    }
    return true;
}

bool pl::save(Playlist* p) {
    clear_selected(p);
    sort_by(p, "fn");
    json out;
    out["name"] = util::json_pack_str(p->name);
    out["sort"] = util::json_pack_str(p->sorting);
    out["sort_rev"] = p->reserve_sorting;
    auto content = json::array();
    content.get_ptr<json::array_t*>()->reserve(p->mus.size());
    for (auto mit = p->mus.begin(); mit != p->mus.end(); mit++) {
        audio::Music* m = *mit;
        content.push_back({
            { "dur", m->dur },
            { "fp", util::json_pack_str(m->full_path) },
            { "mod", m->last_mod },
            { "sz", m->file_size },
            { "tp", (int)m->type },
            });
    }
    out["content"] = content;
    p->changed = false;
    // TF_INFO(<< out);
    std::string out_str(out.dump());
    return write_file(full_path_for_playlist(p->path).c_str(), out_str.data(), out_str.size());
}

void pl::unload_playlists(bool rage) {
    audio::au->cur_stop();
    audio_clear_cache();
    for (auto it = pl::pls->begin(); it != pl::pls->end(); it++) {
        Playlist* p = *it;
        for (auto mit = (*it)->mus.begin(); mit != (*it)->mus.end(); mit++)
            audio::au->mus_close(*mit);
        if (!rage && p->changed)
            save(p);
        tf::dl(p);
    }
}

void pl::audio_clear_cache() {
    for (auto it = audio::au->cache.begin(); it != audio::au->cache.end(); it++) {
        audio::au->mus_close(*it);
    }
    audio::au->cache.clear();
}

void pl::play_selected(Playlist* p) {
    ui::fix_selected_pl();
    if (p->selected.size() == 0)
        return;
    // Maybe clear cache after started playing???
    audio_clear_cache();
    audio::Music* mus = p->mus[p->selected[0]];
    mus_open_file(mus);
    audio::au->cache.push_back(mus);
    audio::au->force_play_cache();
    check_music_mod(mus);
    int cnt = 0;
    for (auto it = p->selected.begin() + 1; it != p->selected.end(); it++) {
        mus = p->mus[*it];
        if (cnt < audio::au->cache_opened_cnt) {
            cnt++;
            mus_open_file(mus);
        }
        check_music_mod(mus);
        audio::au->cache.push_back(mus);
    }
}

void pl::check_music_mod(audio::Music* mus) {
    uint64_t mod_time, file_sz;
    file_mod_time(mus->full_path.c_str(), mod_time, file_sz);
    if (mod_time == 0 || mod_time == mus->last_mod)
        return;
    bool opened = audio::au->mus_opened(mus);
    if ((opened || mus_open_file(mus)) && audio::au->mus_fill_info(mus)) {
        mus->last_mod = mod_time;
        mus->file_size = file_sz;
    }
    if (!opened)
        audio::au->mus_close(mus);
}

int SDLCALL id_compare_by_val_for_del(const int* a, const int* b) {
    if (*a > *b)
        return -1;
    return 1;
}

void pl::remove_selected(Playlist* p) {
    SDL_qsort(p->selected.data(), p->selected.size(), sizeof(int), (SDL_CompareCallback)id_compare_by_val_for_del);
    for (auto it = p->selected.begin(); it != p->selected.end(); it++) {
        audio::Music* m = p->mus[*it];
        // TODO: actually support zombies
        if (audio::au->cur_mus == m) {
            audio::au->cur_stop();
            audio::au->cur_mus = nullptr;
            audio::au->mus_close(m);
        }
        else {
            auto mit = std::find(audio::au->cache.begin(), audio::au->cache.end(), m);
            if (mit != audio::au->cache.end()) {
                audio::au->cache.erase(mit);
                audio::au->mus_close(m);
            }
        }
        p->mus.erase(p->mus.begin() + (size_t)(*it));
    }
    p->changed = true;
    p->selected.clear();
    ui::update_meta_info();
}

void pl::clear_selected(Playlist* p) {
    for (auto it = p->selected.begin(); it != p->selected.end(); it++) {
        p->mus[*it]->selected = false;
    }
    p->selected.clear();
    ui::update_meta_info();
}

void pl::remember_selected(Playlist* p) {
    p->remembering.reserve(p->selected.size());
    for (auto it = p->selected.begin(); it != p->selected.end(); it++) {
        p->remembering.push_back(p->mus[*it]);
    }
}

void pl::unremember_selected(Playlist* p) {
    if (p->selected.size() != p->remembering.size())
        TF_UNREACHABLE();
    auto it2 = p->selected.begin();
    for (auto it = p->remembering.begin(); it != p->remembering.end(); it++) {
        auto need_it = std::find(p->mus.begin(), p->mus.end(), *it);
        if (need_it == p->mus.end())
            TF_UNREACHABLE();
        auto index = std::distance(p->mus.begin(), need_it);
        *it2 = (int)index;
        it2++;
    }
    p->remembering.clear();
}

void pl::fill_cache() {
    // TODO: improve
    Playlist* p = ui::get_last_pl(2);
    if (!p)
        return;
    if (p->mus.size() < 5)
        return;
    while (audio::au->cache.size() < 2) {
        audio::Music* m = p->mus[SDL_rand((Sint32)p->mus.size())];
        if (audio::au->cur_mus == m || std::find(audio::au->cache.begin(), audio::au->cache.end(), m) != audio::au->cache.end())
            continue;
        mus_open_file(m);
        audio::au->cache.push_back(m);
    }
}

void pl::select_all(Playlist* p) {
    p->selected.clear();
    p->selected.reserve(p->mus.size());
    for (int i = 0; i < p->mus.size(); i++) {
        p->selected.push_back(i);
        p->mus[i]->selected = true;
    }
    ui::update_meta_info();
}
