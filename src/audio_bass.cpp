#include <lbs.hpp>
#if ENABLE_BASS
#include <audio_base.hpp>
#include <new.hpp>
#include <log.hpp>
#include <break.hpp>
#include <playlist.hpp>
#include <algorithm>
#include <SDL3/SDL.h>
#if 1
#define BASS_LOAD_FUNC(func_name) do { \
    *(void**)&bass.func_name = (void*)SDL_LoadFunction(bass.handle, #func_name); \
    if (!bass.func_name) { \
        TF_ERROR(<< "Failed to load " << lib_name << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(bass.handle); \
        return; \
    } \
} while (0)
#endif

namespace audio {
    struct BASSApi {
        SDL_SharedObject* handle;
    };

    class AudioBASS : public AudioBase {
        protected:
        BASSApi bass;
        public:

        AudioBASS() : AudioBase() {
            lib_name = "BASS";
            const char* lib_path = IS_WIN ? "bass.dll" : "libbass.so";
            bass.handle = SDL_LoadObject(lib_path);
            if (!bass.handle) {
                TF_WARN(<< "Failed to load " << lib_name << " library (" << SDL_GetError() << ")");
                return;
            }
            TF_INFO(<< "BASS inited successfully");
            inited = true;
        }

        bool dev_open() {
            dev_opened = true;
            return true;
        }

        void dev_close() {
            dev_opened = false;
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            return false;
        }

        void mus_close(Music* mus) {
            if (!mus->h1)
                return;
            mus->h1 = nullptr;
        }

        void force_play_cache() {
            if (cache.size() == 0)
                return;
        }
    
        bool mus_fill_info(Music* mus) {
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

        bool cur_paused() {
            return false;
        }

        void update_volume() {    
        }

        ~AudioBASS() {
            if (!inited)
                return;
            if (dev_opened)
                dev_close();
            inited = false;
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
            SDL_UnloadObject(bass.handle);
        }
    };
}

audio::AudioBase* audio::create_bass() {
    return (audio::AudioBase*)tf::nw<audio::AudioBASS>();
}
#endif
