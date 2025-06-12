#include <playlist.hpp>
#include <log.hpp>
#include <new.hpp>
#include <json_util.hpp>
#include <conf.hpp>
#include <json.hpp>
#include <SDL3/SDL.h>

using nlohmann::json;

namespace app {
    tf::str get_data_path();
}

namespace pl {
    tf::vec<Playlist*>* pls;

    bool load_pl_from_fp(const tf::str& fp);
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
    p->name = util::json_str(d["name"]);
    p->changed = false;
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
            bool ret;
            if (1)
                ret = load_pl_from_fp(app::get_data_path() + file_name);
            else
                ret = load_pl_from_fp(file_name);
            if (ret) {
                pl::pls->at(pl::pls->size() - 1)->path = file_name;
            }
        }
    }
}

void pl::unload_playlists() {
    for (auto it = pl::pls->begin(); it != pl::pls->end(); it++) {
        tf::dl(*it);
    }
}
