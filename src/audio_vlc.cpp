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
#define LIBVLC_API
#if 1
typedef struct libvlc_instance_t libvlc_instance_t;
typedef struct libvlc_media_player_t libvlc_media_player_t;
typedef struct libvlc_media_t libvlc_media_t;
typedef int64_t libvlc_time_t;

#define VLC_LOAD_FUNC(func_name) do { \
    *(void**)&vlc.func_name = (void*)SDL_LoadFunction(vlc.handle, #func_name); \
    if (!vlc.func_name) { \
        TF_ERROR(<< "Failed to load " << lib_name << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(vlc.handle); \
        return; \
    } \
} while (0)
#endif
#define VLC_ERROR() tf::nfstr(vlc.libvlc_errmsg())

namespace audio {
    struct VLCApi {
        SDL_SharedObject* handle;
        libvlc_instance_t* (LIBVLC_API *libvlc_new)(int, const char* const*);
        void (LIBVLC_API *libvlc_release)(libvlc_instance_t*);
        const char* (LIBVLC_API *libvlc_errmsg)();
        void (LIBVLC_API *libvlc_set_app_id)(libvlc_instance_t*, const char*, const char*, const char*);
        libvlc_media_t* (LIBVLC_API *libvlc_media_new_path)(const char*);
        void (LIBVLC_API *libvlc_media_release)(libvlc_media_t*);
        int (LIBVLC_API *libvlc_media_player_play)(libvlc_media_player_t*);
        bool (LIBVLC_API *libvlc_media_player_is_playing)(libvlc_media_player_t*);
        libvlc_time_t (LIBVLC_API *libvlc_media_player_get_time)(libvlc_media_player_t*);
        int (LIBVLC_API *libvlc_media_player_stop)(libvlc_media_player_t*);
        void (LIBVLC_API *libvlc_media_player_release)(libvlc_media_player_t*);
    };

    class AudioVLC : public AudioBase {
        protected:
        VLCApi vlc;
        libvlc_instance_t* inst;
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
            VLC_LOAD_FUNC(libvlc_new);
            VLC_LOAD_FUNC(libvlc_release);
            VLC_LOAD_FUNC(libvlc_errmsg);
            VLC_LOAD_FUNC(libvlc_set_app_id);
            VLC_LOAD_FUNC(libvlc_media_new_path);
            VLC_LOAD_FUNC(libvlc_media_release);
            VLC_LOAD_FUNC(libvlc_media_player_play);
            VLC_LOAD_FUNC(libvlc_media_player_is_playing);
            VLC_LOAD_FUNC(libvlc_media_player_get_time);
            VLC_LOAD_FUNC(libvlc_media_player_stop);
            VLC_LOAD_FUNC(libvlc_media_player_release);
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
            dev_opened = true;
            return true;
        }

        void dev_close() {
            dev_opened = false;
        }

        void dev_fill_arr(tf::vec<tf::str>& arr) {
            arr.push_back("Default");
        }

        void force_play_cache() {
            if (cache.size() == 0)
                return;
            pl::fill_cache();
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            return true;
        }

        void mus_close(Music* mus) {
            if (!mus->h1)
                return;
            mus->h1 = nullptr;
        }
    
        bool mus_fill_info(Music* mus) {
            return false;
            return true;
        }

        void update() {

        }

        void cur_stop() {

        }

        float cur_get_pos() {
            return 0.f;
        }

        void cur_set_pos(float pos) {

        }

        void cur_pause() {

        }

        void cur_resume() {

        }

        bool cur_stopped() {
            return true;
        }

        bool cur_paused() {
            return paused;
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
