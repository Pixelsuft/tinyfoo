#include <playlist.hpp>
#include <log.hpp>
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

namespace pl {
    tf::vec<Playlist*>* pls;

    bool load_pl_from_fp(const tf::str& fp);
    void add_file_by_fp(Playlist*, const char* fp);
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
    p->last_sel = 0;
    p->changed = false;
    sort_by(p, p->sorting.c_str());
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

void pl::add_file_by_fp(Playlist* p, const char* fp) {
    // TF_INFO(<< "Adding file " << fp);
    audio::Music* m = tf::nw<audio::Music>();
    m->full_path = fp;
    auto t1_find = m->full_path.rfind('\\') + 1;
    auto t2_find = m->full_path.rfind('/') + 1;
    if (t1_find >= 0 && t1_find < m->full_path.size())
        m->fn = m->full_path.substr(t1_find);
    else if (t2_find >= 0 && t2_find < m->full_path.size())
        m->fn = m->full_path.substr(t2_find);
    else
        m->fn = m->full_path;
    t1_find = m->fn.rfind('.');
    if (t1_find >= 0 && t1_find < m->full_path.size())
        m->fn = m->fn.substr(0, t1_find);
    for (auto it = p->mus.begin(); it != p->mus.end(); it++) {
        if (util::compare_paths((*it)->full_path, m->full_path)) {
            TF_WARN(<< "File already in playlist (" << fp << ")");
            tf::dl(m);
            return;
        }
    }
    if (!audio::au->mus_open_fp(m, fp)) {
        TF_WARN(<< "Failed to add file " << fp);
        tf::dl(m);
        return;
    }
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

void pl::sort_by(Playlist* p, const char* what) {
    if (SDL_strlen(what) == 0 || !SDL_strcmp(what, "none"))
        return;
    if (!SDL_strcmp(what, "fn"))
        SDL_qsort(p->mus.data(), p->mus.size(), sizeof(audio::Music*), (SDL_CompareCallback)mus_compare_by_name);
    else if (!SDL_strcmp(what, "reverse"))
        std::reverse(p->mus.begin(), p->mus.end());
    else
        TF_ERROR(<< "Unkown playlist sort by (" << what << ")");
}

bool pl::save(Playlist* p) {
    sort_by(p, "fn");
    json out;
    out["name"] = util::json_pack_str(p->name);
    out["sort"] = util::json_pack_str(p->sorting);
    auto content = json::array();
    content.get_ptr<json::array_t*>()->reserve(p->mus.size());
    for (auto mit = p->mus.begin(); mit != p->mus.end(); mit++) {
        audio::Music* m = *mit;
        content.push_back({
            { "fp", util::json_pack_str(m->full_path) },
            { "fn", util::json_pack_str(m->fn) },
            { "dur", m->dur },
            });
    }
    out["content"] = content;
    p->changed = false;
    // TF_INFO(<< out);
    std::string test(out.dump());
    TF_INFO(<< test);
    return true;
}

void pl::unload_playlists() {
    for (auto it = pl::pls->begin(); it != pl::pls->end(); it++) {
        Playlist* p = *it;
        if (p->changed)
            save(p);
        tf::dl(p);
    }
}

void pl::play_selected(Playlist* p) {

}

int SDLCALL id_compare_by_val_for_del(const int* a, const int* b) {
    if (*a > *b)
        return -1;
    return 1;
}

void pl::remove_selected(Playlist* p) {
    // TODO: handle currently played music properly
    SDL_qsort(p->selected.data(), p->selected.size(), sizeof(int), (SDL_CompareCallback)id_compare_by_val_for_del);
    for (auto it = p->selected.begin(); it != p->selected.end(); it++) {
        p->mus.erase(p->mus.begin() + (size_t)(*it));
    }
    p->changed = true;
    p->selected.clear();
}
