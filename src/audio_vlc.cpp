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
#endif

namespace audio {
    struct VLCApi {
        SDL_SharedObject* handle;
    };

    class AudioVLC : public AudioBase {
        protected:
        VLCApi vlc;
        public:
        float pause_pos;
        bool stopped;
        bool paused;

        AudioVLC() : AudioBase() {
            lib_name = "VLC";
            stopped = false;
            paused = false;
            pause_pos = 0.f;
            if (max_volume <= 0.f)
                max_volume = 1.f;
            const char* lib_path = IS_WIN ? "libvlccore.dll" : "libvlccore.so";
            vlc.handle = SDL_LoadObject(lib_path);
            if (!vlc.handle) {
                TF_ERROR(<< "Failed to load VLC library (" << SDL_GetError() << ")");
                return;
            }
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
            inited = false;
            SDL_UnloadObject(vlc.handle);
        }
    };
}

audio::AudioBase* audio::create_vlc() {
    return (audio::AudioBase*)tf::nw<audio::AudioVLC>();
}
#endif
