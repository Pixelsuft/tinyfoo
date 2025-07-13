#include <lbs.hpp>
#if ENABLE_SOLOUD
#include <audio_base.hpp>
#include <new.hpp>
#include <log.hpp>
#include <break.hpp>
#include <playlist.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <SDL3/SDL.h>

template<int> const char* SoLoudDefaultDllHelper();

template<> const char* SoLoudDefaultDllHelper<4>() {
    return IS_WIN ? "soloud_x86.dll" : "libsoloud_x86.so";
}

template<> const char* SoLoudDefaultDllHelper<8>() {
    return IS_WIN ? "soloud_x64.dll" : "libsoloud_x64.so";
}

namespace audio {
    struct SoLoudApi {
        SDL_SharedObject* handle;
    };

    class AudioSoLoud : public AudioBase {
        protected:
        SoLoudApi sl;
        public:
        float pause_pos;
        bool was_finished;
        bool hooked;
        bool stopped;
        bool paused;

        AudioSoLoud() : AudioBase() {
            lib_name = "SoLoud";
            was_finished = false;
            hooked = false;
            stopped = false;
            paused = false;
            pause_pos = 0.f;
            if (max_volume <= 0.f)
                max_volume = 1.f;
            const char* lib_path = SoLoudDefaultDllHelper<sizeof(size_t)>();
            sl.handle = SDL_LoadObject(lib_path);
            if (!sl.handle) {
                TF_ERROR(<< "Failed to load SoLoud library (" << SDL_GetError() << ")");
                return;
            }
            inited = true;
        }

        bool dev_open() {
            return false;
            dev_opened = true;
            return true;
        }

        void dev_close() {
            dev_opened = false;
        }

        void dev_fill_arr(tf::vec<tf::str>& arr) {
            arr.push_back("Default");
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            mus->h1 = nullptr;
            if (!mus->h1) {
                TF_ERROR(<< "Failed to open music \"" << fp << "\" (" << SDL_GetError() << ")");
                return false;
            }
            return true;
        }

        void mus_close(Music* mus) {
            if (!mus->h1)
                return;
            mus->h1 = nullptr;
        }

        void force_play_cache() {
            if (cache.size() == 0)
                return;
            pl::fill_cache();
        }
    
        bool mus_fill_info(Music* mus) {
            return false;
        }

        void update() {
            if (was_finished) {
                was_finished = false;
                hooked = false;
            }
        }

        void cur_stop() {
            bool was_paused = cur_paused();
            paused = false;
            stopped = true;
            pl::fill_cache();
        }

        float cur_get_pos() {
            if (cur_paused())
                return pause_pos;
            return 0.f;
        }

        void cur_set_pos(float pos) {
            if (!cur_mus || !hooked)
                return;
            pos = tf::clamp(pos, 0.f, cur_mus->dur);
            if (paused) {
                pause_pos = pos;
                return;
            }
        }

        void cur_pause() {
            if (paused || !cur_mus)
                return;
            pause_pos = cur_get_pos();
            paused = true;
        }

        void cur_resume() {
            if (!paused || !cur_mus)
                return;
            paused = false;
            stopped = false;
        }

        bool cur_stopped() {
            return !hooked && !paused;
        }

        bool cur_paused() {
            return paused;
        }

        void update_volume() {
            volume = tf::clamp(volume, 0.f, std::min(max_volume, 1.f));    
        }

        ~AudioSoLoud() {
            if (!inited)
                return;
            if (dev_opened)
                dev_close();
            inited = false;
            SDL_UnloadObject(sl.handle);
        }
    };
}

audio::AudioBase* audio::create_soloud() {
    return (audio::AudioBase*)tf::nw<audio::AudioSoLoud>();
}
#endif
