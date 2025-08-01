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
#define LIBVLC_API
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
    libvlc_media_do_interact    = 0x20
} libvlc_media_parse_flag_t;
typedef enum libvlc_media_parse_flag_old_t {
    libvlc_media_parse_local_old    = 0x00,
    libvlc_media_parse_network_old  = 0x01,
    libvlc_media_fetch_local_old    = 0x02,
    libvlc_media_fetch_network_old  = 0x04,
    libvlc_media_do_interact_old    = 0x08
} libvlc_media_parse_flag_old_t;
typedef enum libvlc_media_parsed_status_t {
    libvlc_media_parsed_status_none,
    libvlc_media_parsed_status_pending,
    libvlc_media_parsed_status_skipped,
    libvlc_media_parsed_status_failed,
    libvlc_media_parsed_status_timeout,
    libvlc_media_parsed_status_cancelled,
    libvlc_media_parsed_status_done
} libvlc_media_parsed_status_t;
typedef enum libvlc_media_parsed_status_old_t {
    libvlc_media_parsed_status_skipped_old = 1,
    libvlc_media_parsed_status_failed_old,
    libvlc_media_parsed_status_timeout_old,
    libvlc_media_parsed_status_done_old
} libvlc_media_parsed_status_old_t;

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
#define VLC_IS_PLAYING() (is4 ? vlc.libvlc_media_player_is_playing(mp) : (vlc.libvlc_media_player_is_playing_old(mp) > 0))

// VLC doesn't seem to support fading API
// TODO: fix position get/set lags

namespace audio {
    struct VLCApi {
        SDL_SharedObject* handle;
        const char* (LIBVLC_API *libvlc_get_version)();        
        libvlc_instance_t* (LIBVLC_API *libvlc_new)(int, const char* const*);
        void (LIBVLC_API *libvlc_release)(libvlc_instance_t*);
        const char* (LIBVLC_API *libvlc_errmsg)();
        void (LIBVLC_API *libvlc_set_app_id)(libvlc_instance_t*, const char*, const char*, const char*);
        libvlc_media_t* (LIBVLC_API *libvlc_media_new_path)(const char*);
        libvlc_media_t* (LIBVLC_API *libvlc_media_new_path_old)(libvlc_instance_t*, const char*);
        void (LIBVLC_API *libvlc_media_release)(libvlc_media_t*);
        libvlc_time_t (LIBVLC_API *libvlc_media_get_duration)(libvlc_media_t*);
        libvlc_media_player_t* (LIBVLC_API *libvlc_media_player_new)(libvlc_instance_t*);
        int (LIBVLC_API *libvlc_media_player_play)(libvlc_media_player_t*);
        bool (LIBVLC_API *libvlc_media_player_is_playing)(libvlc_media_player_t*);
        int (LIBVLC_API *libvlc_media_player_is_playing_old)(libvlc_media_player_t*);
        libvlc_time_t (LIBVLC_API *libvlc_media_player_get_time)(libvlc_media_player_t*);
        int (LIBVLC_API *libvlc_media_player_set_time)(libvlc_media_player_t*, libvlc_time_t, bool);
        void (LIBVLC_API *libvlc_media_player_set_time_old)(libvlc_media_player_t*, libvlc_time_t);
        int (LIBVLC_API *libvlc_media_player_stop_async)(libvlc_media_player_t*);
        void (LIBVLC_API *libvlc_media_player_stop)(libvlc_media_player_t*);
        void (LIBVLC_API *libvlc_media_player_release)(libvlc_media_player_t*);
        void (LIBVLC_API *libvlc_media_player_set_pause)(libvlc_media_player_t*, int);
        void (LIBVLC_API *libvlc_media_player_set_media)(libvlc_media_player_t*, libvlc_media_t*);
        int (LIBVLC_API *libvlc_media_parse_request)(libvlc_instance_t*, libvlc_media_t*, libvlc_media_parse_flag_t, int);
        int (LIBVLC_API *libvlc_media_parse_with_options)(libvlc_media_t*, libvlc_media_parse_flag_old_t, int);
        libvlc_media_parsed_status_t (LIBVLC_API *libvlc_media_get_parsed_status)(libvlc_media_t*);
        libvlc_media_parsed_status_old_t (LIBVLC_API *libvlc_media_get_parsed_status_old)(libvlc_media_t*);
        int (LIBVLC_API *libvlc_audio_set_volume)(libvlc_media_player_t*, int);
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
        bool real_paused;
        bool fast_seek;
        bool is4;

        AudioVLC() : AudioBase() {
            inst = nullptr;
            lib_name = "VLC";
            stopped = false;
            paused = false;
            real_paused = false;
            fast_seek = conf::read_bool("vlc", "fast_seek", true);
            pause_pos = 0.f;
            if (max_volume <= 0.f)
                max_volume = 1.f;
            const char* lib_path = IS_WIN ? "libvlc.dll" : "libvlc.so";
            vlc.handle = SDL_LoadObject(lib_path);
            if (!IS_WIN && !vlc.handle)
                vlc.handle = SDL_LoadObject("libvlc.so.5");
            if (!vlc.handle) {
                TF_ERROR(<< "Failed to load VLC library (" << SDL_GetError() << ")");
                return;
            }
            VLC_LOAD_FUNC(libvlc_get_version);
            const char* ver_str = vlc.libvlc_get_version();
            if (!ver_str || ver_str[0] != '4')
                TF_WARN(<< "Incorrect libvlc version (4 is required, got " << tf::nfstr(ver_str) << "), expect problems");
            is4 = SDL_atoi(ver_str) >= 4;
            VLC_LOAD_FUNC(libvlc_new);
            VLC_LOAD_FUNC(libvlc_release);
            VLC_LOAD_FUNC(libvlc_errmsg);
            VLC_LOAD_FUNC(libvlc_set_app_id);
            VLC_LOAD_FUNC(libvlc_media_new_path);
            VLC_LOAD_FUNC(libvlc_media_release);
            VLC_LOAD_FUNC(libvlc_media_get_duration);
            VLC_LOAD_FUNC(libvlc_media_player_new);
            VLC_LOAD_FUNC(libvlc_media_player_play);
            VLC_LOAD_FUNC(libvlc_media_player_is_playing);
            VLC_LOAD_FUNC(libvlc_media_player_get_time);
            VLC_LOAD_FUNC(libvlc_media_player_set_time);
            if (is4)
                VLC_LOAD_FUNC(libvlc_media_player_stop_async);
            else
                VLC_LOAD_FUNC(libvlc_media_player_stop);
            VLC_LOAD_FUNC(libvlc_media_player_release);
            VLC_LOAD_FUNC(libvlc_media_player_set_pause);
            VLC_LOAD_FUNC(libvlc_media_player_set_media);
            if (is4)
                VLC_LOAD_FUNC(libvlc_media_parse_request);
            else
                VLC_LOAD_FUNC(libvlc_media_parse_with_options);
            VLC_LOAD_FUNC(libvlc_media_get_parsed_status);
            VLC_LOAD_FUNC(libvlc_audio_set_volume);
            *(void**)&vlc.libvlc_media_new_path_old = (void*)vlc.libvlc_media_new_path;
            *(void**)&vlc.libvlc_media_player_is_playing_old = (void*)vlc.libvlc_media_player_is_playing;
            *(void**)&vlc.libvlc_media_player_set_time_old = (void*)vlc.libvlc_media_player_set_time;
            *(void**)&vlc.libvlc_media_get_parsed_status_old = (void*)vlc.libvlc_media_get_parsed_status;
            inst = vlc.libvlc_new(0, nullptr);
            if (!inst) {
                TF_ERROR(<< "Failed to init VLC (" << VLC_ERROR() << ")");
                SDL_UnloadObject(vlc.handle);
                return;
            }
            vlc.libvlc_set_app_id(inst, "com.pixelsuft.tinyfoo", "1.0.0", "tinyfoo");
            TF_INFO(<< "VLC inited");
            inited = true;
        }

        bool dev_open() {
            mp = vlc.libvlc_media_player_new(inst);
            if (!mp) {
                TF_ERROR(<< "Failed create VLC media player (" << VLC_ERROR() << ")");
                return false;
            }
            TF_INFO(<< "VLC media player created");
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
            if (cache[0] == cur_mus) {
                if (stopped) {
                    cur_mus = nullptr;
                    force_play_cache();
                    pl::fill_cache();
                    return;
                }
                cache.erase(cache.begin());
                if (paused) {
                    pause_pos = 0.f;
                    cur_resume();
                }
                else
                    cur_set_pos(0.f);
                update_volume();
                pl::fill_cache();
                return;
            }
            stopped = false;
            Music* prev = cur_mus;
            cur_mus = cache[0];
            cache.erase(cache.begin());
            cur_mus->cached = false;
            paused = real_paused = false;
            pl::mus_open_file(cur_mus);
            vlc.libvlc_media_player_set_media(mp, cur_h);
            if (vlc.libvlc_media_player_play(mp) < 0)
                TF_WARN(<< "Failed to play media (" << VLC_ERROR() << ")");
            if (!is4) {
                // WTF
                while (!VLC_IS_PLAYING()) {}
            }
            update_volume();
            if (prev && prev != cur_mus && std::find(cache.begin(), cache.end(), prev) == cache.end())
                mus_close(prev);
            pl::fill_cache();
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            libvlc_media_t* med = is4 ? vlc.libvlc_media_new_path(fp) : vlc.libvlc_media_new_path_old(inst, fp);
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
            if ((is4 ? vlc.libvlc_media_parse_request(inst, med_h, libvlc_media_parse_local, 0) : vlc.libvlc_media_parse_with_options(med_h, libvlc_media_parse_local_old, 0)) < 0) {
                TF_WARN(<< "Failed to send music parse request (" << VLC_ERROR() << ")");
                return false;
            }
            if (is4) {
                libvlc_media_parsed_status_t status = libvlc_media_parsed_status_pending;
                while (status == libvlc_media_parsed_status_pending)
                    status = vlc.libvlc_media_get_parsed_status(med_h);
                if (status != libvlc_media_parsed_status_done) {
                    TF_WARN(<< "Failed parse music information (" << VLC_ERROR() << ")");
                    return false;
                }
            }
            else {
                // TODO: FIXME
                libvlc_media_parsed_status_old_t status = libvlc_media_parsed_status_timeout_old;
                while (status != libvlc_media_parsed_status_done_old)
                    status = vlc.libvlc_media_get_parsed_status_old(med_h);
                if (status != libvlc_media_parsed_status_done_old) {
                    TF_WARN(<< "Failed parse music information (" << VLC_ERROR() << ")");
                    return false;
                }
            }
            auto temp_dur = vlc.libvlc_media_get_duration(med_h);
            if (temp_dur < 0) {
                TF_WARN(<< "Failed to get music duration (" << VLC_ERROR() << ")");
                return false;
            }
            mus->dur = (float)temp_dur / 1000.f;
            audio::mus_fill_format_default(mus);
            return true;
        }

        void update() {
            if (real_paused && VLC_IS_PLAYING())
                real_paused = false;
            if (cur_mus && !paused && !VLC_IS_PLAYING()) {
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
            if (is4) {
                // Works fine
                if (vlc.libvlc_media_player_stop_async(mp) < 0)
                    TF_WARN(<< "Failed to stop audio (" << VLC_ERROR() << ")");
            }
            else
                vlc.libvlc_media_player_stop(mp);
            pl::fill_cache();
        }

        float cur_get_pos() {
            if (real_paused)
                return pause_pos;
            if (!cur_mus || stopped || !VLC_IS_PLAYING())
                return 0.f;
            auto ret = vlc.libvlc_media_player_get_time(mp);
            if (ret < 0) {
                TF_WARN(<< "Failed to get music position (" << VLC_ERROR() << ")");
                return 0.f;
            }
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
            if (is4) {
                if (vlc.libvlc_media_player_set_time(mp, (libvlc_time_t)(pos * 1000.f), fast_seek) < 0)
                    TF_WARN(<< "Failed to set audio position (" << VLC_ERROR() << ")");
            }
            else
                vlc.libvlc_media_player_set_time_old(mp, (libvlc_time_t)(pos * 1000.f));
        }

        void cur_pause() {
            if (!cur_mus || paused)
                return;
            pause_pos = cur_get_pos();
            paused = true;
            real_paused = true;
            vlc.libvlc_media_player_set_pause(mp, 1);
        }

        void cur_resume() {
            if (!cur_mus || !paused)
                return;
            paused = false;
            stopped = false;
            vlc.libvlc_media_player_set_pause(mp, 0);
            if (is4) {
                if (vlc.libvlc_media_player_set_time(mp, (libvlc_time_t)(pause_pos * 1000.f), fast_seek) < 0)
                    TF_WARN(<< "Failed to set audio resume position (" << VLC_ERROR() << ")");
            }
            else
                vlc.libvlc_media_player_set_time_old(mp, (libvlc_time_t)(pause_pos * 1000.f));
        }

        bool cur_stopped() {
            return !cur_mus || (!paused && !VLC_IS_PLAYING());
        }

        bool cur_paused() {
            return cur_mus && real_paused;
        }

        void update_volume() {
            volume = tf::clamp(volume, 0.f, max_volume);
            if (vlc.libvlc_audio_set_volume(mp, (int)(volume * 100.f)) < 0)
                TF_WARN(<< "Failed to set audio volume (" << VLC_ERROR() << ")");
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
