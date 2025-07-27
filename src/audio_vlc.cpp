#include <lbs.hpp>
#if ENABLE_VLC
#include <audio.hpp>
#include <new.hpp>
#include <log.hpp>
#include <break.hpp>
#include <playlist.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <SDL3/SDL.h>
#if 1
#define LIBVLC_API SDLCALL
typedef struct libvlc_instance_t libvlc_instance_t;
typedef struct libvlc_media_player_t libvlc_media_player_t;
typedef struct libvlc_media_t libvlc_media_t;
typedef int64_t libvlc_time_t;
typedef enum libvlc_media_parse_flag_t {
    libvlc_media_parse_local    = 0x01,
    libvlc_media_parse_network  = 0x02,
    libvlc_media_parse_forced   = 0x04,
    libvlc_media_fetch_local    = 0x08,
    libvlc_media_fetch_network  = 0x10,
    libvlc_media_do_interact    = 0x20,
} libvlc_media_parse_flag_t;

#define VLC_LOAD_FUNC(func_name) do { \
    *(void**)&vlc.func_name = (void*)SDL_LoadFunction(vlc.handle, #func_name); \
    if (!vlc.func_name) { \
        TF_ERROR(<< "Failed to load " << lib_name << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(vlc.handle); \
        return; \
    } \
} while (0)
#endif
#ifdef mus_h
#undef mus_h
#endif
#ifdef cur_h
#undef cur_h
#endif
#define cur_h ((libvlc_media_t*)cur_mus->h1)
#define med_h ((libvlc_media_t*)mus->h1)
#define VLC_ERROR() tf::nfstr(vlc.libvlc_errmsg())

// VLC doesn't seem to support fading API

namespace audio {
    struct VLCApi {
        SDL_SharedObject* handle;
        const char* (LIBVLC_API *libvlc_get_version)();        
        libvlc_instance_t* (LIBVLC_API *libvlc_new)(int, const char* const*);
        void (LIBVLC_API *libvlc_release)(libvlc_instance_t*);
        const char* (LIBVLC_API *libvlc_errmsg)();
        void (LIBVLC_API *libvlc_set_app_id)(libvlc_instance_t*, const char*, const char*, const char*);
        libvlc_media_t* (LIBVLC_API *libvlc_media_new_path)(const char*);
        void (LIBVLC_API *libvlc_media_release)(libvlc_media_t*);
        libvlc_time_t (LIBVLC_API *libvlc_media_get_duration)(libvlc_media_t*);
        libvlc_media_player_t* (LIBVLC_API *libvlc_media_player_new)(libvlc_instance_t*);
        libvlc_media_player_t* (LIBVLC_API *libvlc_media_player_new_from_media)(libvlc_instance_t*, libvlc_media_t*);
        int (LIBVLC_API *libvlc_media_player_play)(libvlc_media_player_t*);
        bool (LIBVLC_API *libvlc_media_player_is_playing)(libvlc_media_player_t*);
        libvlc_time_t (LIBVLC_API *libvlc_media_player_get_time)(libvlc_media_player_t*);
        int (LIBVLC_API *libvlc_media_player_set_time)(libvlc_media_player_t*, libvlc_time_t, bool);
        int (LIBVLC_API *libvlc_media_player_stop_async)(libvlc_media_player_t*);
        void (LIBVLC_API *libvlc_media_player_release)(libvlc_media_player_t*);
        libvlc_time_t (LIBVLC_API *libvlc_media_player_get_length)(libvlc_media_player_t*);
        void (LIBVLC_API *libvlc_media_player_pause)(libvlc_media_player_t*);
        void (LIBVLC_API *libvlc_media_player_set_media)(libvlc_media_player_t*, libvlc_media_t*);
        void (LIBVLC_API *libvlc_audio_set_format)(libvlc_media_player_t*, const char*, unsigned, unsigned);
        int (LIBVLC_API *libvlc_media_parse_request)(libvlc_instance_t*, libvlc_media_t*, libvlc_media_parse_flag_t, int);
        void (LIBVLC_API *libvlc_media_parse_stop)(libvlc_instance_t*, libvlc_media_t*);
    };

    class AudioVLC : public AudioBase {
        protected:
        VLCApi vlc;
        libvlc_instance_t* inst;
        libvlc_media_player_t* mp;
        public:
        float pause_pos;
        bool stopped;
        bool paused;

        AudioVLC() : AudioBase() {
            inst = nullptr;
            lib_name = "VLC";
            stopped = false;
            paused = false;
            pause_pos = 0.f;
            if (max_volume <= 0.f)
                max_volume = 1.f;
            const char* lib_path = IS_WIN ? "libvlc.dll" : "libvlc.so";
            vlc.handle = SDL_LoadObject(lib_path);
            if (!vlc.handle) {
                TF_ERROR(<< "Failed to load VLC library (" << SDL_GetError() << ")");
                return;
            }
            VLC_LOAD_FUNC(libvlc_get_version);
            const char* ver_str = vlc.libvlc_get_version();
            // Maybe support version 3?
            if (!ver_str || ver_str[0] != '4')
                TF_WARN(<< "Incorrect libvlc version (4 is required, got " << tf::nfstr(ver_str) << "), expect problems");
            VLC_LOAD_FUNC(libvlc_new);
            VLC_LOAD_FUNC(libvlc_release);
            VLC_LOAD_FUNC(libvlc_errmsg);
            VLC_LOAD_FUNC(libvlc_set_app_id);
            VLC_LOAD_FUNC(libvlc_media_new_path);
            VLC_LOAD_FUNC(libvlc_media_release);
            VLC_LOAD_FUNC(libvlc_media_get_duration);
            VLC_LOAD_FUNC(libvlc_media_player_new);
            VLC_LOAD_FUNC(libvlc_media_player_new_from_media);
            VLC_LOAD_FUNC(libvlc_media_player_play);
            VLC_LOAD_FUNC(libvlc_media_player_is_playing);
            VLC_LOAD_FUNC(libvlc_media_player_get_time);
            VLC_LOAD_FUNC(libvlc_media_player_set_time);
            VLC_LOAD_FUNC(libvlc_media_player_stop_async);
            VLC_LOAD_FUNC(libvlc_media_player_release);
            VLC_LOAD_FUNC(libvlc_media_player_get_length);
            VLC_LOAD_FUNC(libvlc_media_player_pause);
            VLC_LOAD_FUNC(libvlc_media_player_set_media);
            VLC_LOAD_FUNC(libvlc_audio_set_format);
            VLC_LOAD_FUNC(libvlc_media_parse_request);
            VLC_LOAD_FUNC(libvlc_media_parse_stop);
            // TODO: maybe this should be in dev_open?
            inst = vlc.libvlc_new(0, nullptr);
            if (!inst) {
                TF_ERROR(<< "Failed to init VLC (" << VLC_ERROR() << ")");
                SDL_UnloadObject(vlc.handle);
                return;
            }
            vlc.libvlc_set_app_id(inst, "com.pixelsuft.tinyfoo", "1.0.0", "tinyfoo");
            TF_INFO(<< "VLC successfully created");
            inited = true;
        }

        bool dev_open() {
            mp = vlc.libvlc_media_player_new(inst);
            if (!mp) {
                TF_ERROR(<< "Failed create VLC media player (" << VLC_ERROR() << ")");
                return false;
            }
            dev_opened = true;
            return true;
        }

        void dev_close() {
            vlc.libvlc_media_player_release(mp);
            dev_opened = false;
        }

        void dev_fill_arr(tf::vec<tf::str>& arr) {
            arr.push_back("Default");
        }

        void force_play_cache() {
            if (cache.size() == 0)
                return;
            bool from_rep = false;
            if (cache[0] == cur_mus) {
                if (stopped) {
                    cur_mus = nullptr;
                    force_play_cache();
                    pl::fill_cache();
                    return;
                }
                cache.erase(cache.begin());
                if (paused || 1) {
                    paused = false;
                    update_volume();
                    pl::fill_cache();
                    return;
                }
                from_rep = true;
            }
            stopped = false;
            if (cur_mus && 0) {
                stopped = false;
                paused = false;
                return;
            }
            Music* prev = nullptr;
            if (!from_rep) {
                prev = cur_mus;
                cur_mus = cache[0];
                cache.erase(cache.begin());
            }
            cur_mus->cached = false;
            stopped = false;
            paused = false;
            pl::mus_open_file(cur_mus);
            vlc.libvlc_media_player_set_media(mp, cur_h);
            if (vlc.libvlc_media_player_play(mp) < 0)
                TF_WARN(<< "Failed to play media (" << VLC_ERROR() << ")");
            if (prev && prev != cur_mus && std::find(cache.begin(), cache.end(), prev) == cache.end())
                mus_close(prev);
            pl::fill_cache();
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            libvlc_media_t* med = vlc.libvlc_media_new_path(fp);
            if (!med) {
                TF_ERROR(<< "Failed create media from file (" << VLC_ERROR() << ")");
                return false;
            }
            mus->h1 = (void*)med;
            return true;
        }

        void mus_close(Music* mus) {
            if (!mus->h1)
                return;
            vlc.libvlc_media_release(med_h);
            mus->h1 = nullptr;
        }
    
        bool mus_fill_info(Music* mus) {
            if (vlc.libvlc_media_parse_request(inst, med_h, libvlc_media_parse_local, 0) < 0) {
                TF_WARN(<< "Failed to send music parse request (" << VLC_ERROR() << ")");
                return false;
            }
            libvlc_time_t temp_dur = -1;
            // TODO: handle errors properly?
            while (temp_dur == -1)
                temp_dur = vlc.libvlc_media_get_duration(med_h);
            mus->dur = (float)temp_dur / 1000.f;
            mus->type = Type::NONE;
            // Hacky
            char ext_buf[5];
            if (mus->full_path.size() >= 5) {
                ext_buf[0] = SDL_tolower(mus->full_path[mus->full_path.size() - 5]);
                ext_buf[1] = SDL_tolower(mus->full_path[mus->full_path.size() - 4]);
                ext_buf[2] = SDL_tolower(mus->full_path[mus->full_path.size() - 3]);
                ext_buf[3] = SDL_tolower(mus->full_path[mus->full_path.size() - 2]);
                ext_buf[4] = SDL_tolower(mus->full_path[mus->full_path.size() - 1]);
            }
            else
                ext_buf[0] = ext_buf[1] = ext_buf[2] = ext_buf[3] = ext_buf[4] = '\0';
            if (SDL_memcmp(ext_buf + 1, ".mp3", 4) == 0)
                mus->type = Type::MP3;
            else if (SDL_memcmp(ext_buf + 1, ".ogg", 4) == 0)
                mus->type = Type::OGG;
            else if (SDL_memcmp(ext_buf + 1, ".wav", 4) == 0)
                mus->type = Type::WAV;
            else if (SDL_memcmp(ext_buf, ".flac", 5) == 0)
                mus->type = Type::FLAC;
            return true;
        }

        void update() {
            if (cur_mus && !paused && !vlc.libvlc_media_player_is_playing(mp)) {
                if (!stopped)
                    force_play_cache();
                pl::fill_cache();
                pre_open();
            }
        }

        void cur_stop() {
            if (!cur_mus)
                return;
            stopped = true;
            // TODO: should I acually wait until stop?
            vlc.libvlc_media_player_stop_async(mp);
            pl::fill_cache();
        }

        float cur_get_pos() {
            if (cur_paused())
                return pause_pos;
            if (!cur_mus || stopped || !vlc.libvlc_media_player_is_playing(mp))
                return 0.f;
            auto ret = vlc.libvlc_media_player_get_time(mp);
            return (float)ret / 1000.f;
        }

        void cur_set_pos(float pos) {
            if (!cur_mus)
                return;
            pos = tf::clamp(pos, 0.f, cur_mus->dur);
            if (paused) {
                pause_pos = pos;
                return;
            }
            // TODO
        }

        void cur_pause() {
            if (!cur_mus || paused)
                return;
            pause_pos = cur_get_pos();
            paused = true;
            vlc.libvlc_media_player_pause(mp);
        }

        void cur_resume() {
            if (!cur_mus || !paused)
                return;
            paused = false;
            stopped = false;
            vlc.libvlc_media_player_pause(mp);
            // TODO: seek
        }

        bool cur_stopped() {
            return !cur_mus || (!paused && !vlc.libvlc_media_player_is_playing(mp));
        }

        bool cur_paused() {
            return cur_mus && paused;
        }

        void update_volume() {
            volume = tf::clamp(volume, 0.f, std::min(max_volume, 1.f));
        }

        ~AudioVLC() {
            if (!inited)
                return;
            if (dev_opened)
                dev_close();
            vlc.libvlc_release(inst);
            inited = false;
            SDL_UnloadObject(vlc.handle);
        }
    };
}

audio::AudioBase* audio::create_vlc() {
    return (audio::AudioBase*)tf::nw<audio::AudioVLC>();
}
#endif
