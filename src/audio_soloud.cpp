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
#if 1
typedef void* AudioSource;
typedef void* Soloud;
typedef void* Wav;
typedef void* WavStream;

enum SOLOUD_ENUMS {
	SOLOUD_AUTO = 0,
	SOLOUD_SDL1 = 1,
	SOLOUD_SDL2 = 2,
	SOLOUD_PORTAUDIO = 3,
	SOLOUD_WINMM = 4,
	SOLOUD_XAUDIO2 = 5,
	SOLOUD_WASAPI = 6,
	SOLOUD_ALSA = 7,
	SOLOUD_JACK = 8,
	SOLOUD_OSS = 9,
	SOLOUD_OPENAL = 10,
	SOLOUD_COREAUDIO = 11,
	SOLOUD_OPENSLES = 12,
	SOLOUD_VITA_HOMEBREW = 13,
	SOLOUD_MINIAUDIO = 14,
	SOLOUD_NOSOUND = 15,
	SOLOUD_NULLDRIVER = 16,
	SOLOUD_BACKEND_MAX = 17,
	SOLOUD_CLIP_ROUNDOFF = 1,
	SOLOUD_ENABLE_VISUALIZATION = 2,
	SOLOUD_LEFT_HANDED_3D = 4,
	SOLOUD_NO_FPU_REGISTER_CHANGE = 8,
	BASSBOOSTFILTER_WET = 0,
	BASSBOOSTFILTER_BOOST = 1,
	BIQUADRESONANTFILTER_LOWPASS = 0,
	BIQUADRESONANTFILTER_HIGHPASS = 1,
	BIQUADRESONANTFILTER_BANDPASS = 2,
	BIQUADRESONANTFILTER_WET = 0,
	BIQUADRESONANTFILTER_TYPE = 1,
	BIQUADRESONANTFILTER_FREQUENCY = 2,
	BIQUADRESONANTFILTER_RESONANCE = 3,
	ECHOFILTER_WET = 0,
	ECHOFILTER_DELAY = 1,
	ECHOFILTER_DECAY = 2,
	ECHOFILTER_FILTER = 3,
	FLANGERFILTER_WET = 0,
	FLANGERFILTER_DELAY = 1,
	FLANGERFILTER_FREQ = 2,
	FREEVERBFILTER_WET = 0,
	FREEVERBFILTER_FREEZE = 1,
	FREEVERBFILTER_ROOMSIZE = 2,
	FREEVERBFILTER_DAMP = 3,
	FREEVERBFILTER_WIDTH = 4,
	LOFIFILTER_WET = 0,
	LOFIFILTER_SAMPLERATE = 1,
	LOFIFILTER_BITDEPTH = 2,
	NOISE_WHITE = 0,
	NOISE_PINK = 1,
	NOISE_BROWNISH = 2,
	NOISE_BLUEISH = 3,
	ROBOTIZEFILTER_WET = 0,
	ROBOTIZEFILTER_FREQ = 1,
	ROBOTIZEFILTER_WAVE = 2,
	SFXR_COIN = 0,
	SFXR_LASER = 1,
	SFXR_EXPLOSION = 2,
	SFXR_POWERUP = 3,
	SFXR_HURT = 4,
	SFXR_JUMP = 5,
	SFXR_BLIP = 6,
	SPEECH_KW_SAW = 0,
	SPEECH_KW_TRIANGLE = 1,
	SPEECH_KW_SIN = 2,
	SPEECH_KW_SQUARE = 3,
	SPEECH_KW_PULSE = 4,
	SPEECH_KW_NOISE = 5,
	SPEECH_KW_WARBLE = 6,
	VIC_PAL = 0,
	VIC_NTSC = 1,
	VIC_BASS = 0,
	VIC_ALTO = 1,
	VIC_SOPRANO = 2,
	VIC_NOISE = 3,
	VIC_MAX_REGS = 4,
	WAVESHAPERFILTER_WET = 0,
	WAVESHAPERFILTER_AMOUNT = 1
};

#define SL_LOAD_FUNC(func_name) do { \
    *(void**)&sl.func_name = (void*)SDL_LoadFunction(sl.handle, #func_name); \
    if (!sl.func_name) { \
        TF_ERROR(<< "Failed to load " << lib_name << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(sl.handle); \
        return; \
    } \
} while (0)
#endif
#define SL_ERROR() sl.Soloud_getErrorString(sys, ret)
#define SL_HAS_ERROR(expr) ((expr) != 0)
#define SL_API
#define cur_wav ((WavStream*)cur_mus->h1)
#define mus_wav ((WavStream*)mus->h1)

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
        void (SL_API *Soloud_destroy)(Soloud*);
        Soloud* (SL_API *Soloud_create)();
        int (SL_API *Soloud_initEx)(Soloud*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
        void (SL_API *Soloud_deinit)(Soloud*);
        unsigned int (SL_API *Soloud_getVersion)(Soloud*);
        const char* (SL_API *Soloud_getErrorString)(Soloud*, int);
        unsigned int (SL_API *Soloud_getBackendId)(Soloud*);
        const char* (SL_API *Soloud_getBackendString)(Soloud*);
        unsigned int (SL_API *Soloud_getBackendChannels)(Soloud*);
        unsigned int (SL_API *Soloud_getBackendSamplerate)(Soloud*);
        unsigned int (SL_API *Soloud_getBackendBufferSize)(Soloud*);
        unsigned int (SL_API *Soloud_playEx)(Soloud*, AudioSource*, float, float, int, unsigned int);
        unsigned int (SL_API *Soloud_playBackgroundEx)(Soloud*, AudioSource*, float, int, unsigned int);
        int (SL_API *Soloud_seek)(Soloud*, unsigned int, double);
        void (SL_API *Soloud_stop)(Soloud*, unsigned int);
        void (SL_API *Soloud_stopAll)(Soloud*);
        int (SL_API *Soloud_countAudioSource)(Soloud*, AudioSource*);
        double (SL_API *Soloud_getStreamTime)(Soloud*, unsigned int);
        double (SL_API *Soloud_getStreamPosition)(Soloud*, unsigned int);
        int (SL_API *Soloud_getPause)(Soloud*, unsigned int);
        void (SL_API *Soloud_setPause)(Soloud*, unsigned int, int);
        float (SL_API *Soloud_getVolume)(Soloud*, unsigned int);
        int (SL_API *Soloud_setRelativePlaySpeed)(Soloud*, unsigned int, float);
        void (SL_API *Soloud_setVolume)(Soloud*, unsigned int, float);
        void (SL_API *Soloud_fadeVolume)(Soloud*, unsigned int, float, double);
        void (SL_API *Soloud_schedulePause)(Soloud*, unsigned int, double);
        void (SL_API *Soloud_scheduleStop)(Soloud*, unsigned int, double);
        void (SL_API *WavStream_destroy)(WavStream*);
        WavStream* (SL_API *WavStream_create)();
        int (SL_API *WavStream_load)(WavStream*, const char*);
        double (SL_API *WavStream_getLength)(WavStream*);
        void (SL_API *WavStream_setVolume)(WavStream*, float);
        void (SL_API *WavStream_stop)(WavStream*);
    };

    class AudioSoLoud : public AudioBase {
        protected:
        SoLoudApi sl;
        Soloud* sys;
        public:
        float pause_pos;
        unsigned int ch;
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
            ch = 0;
            const char* lib_path = SoLoudDefaultDllHelper<sizeof(void*)>();
            sl.handle = SDL_LoadObject(lib_path);
            if (!sl.handle) {
                TF_ERROR(<< "Failed to load SoLoud library (" << SDL_GetError() << ")");
                return;
            }
            SL_LOAD_FUNC(Soloud_destroy);
            SL_LOAD_FUNC(Soloud_create);
            SL_LOAD_FUNC(Soloud_initEx);
            SL_LOAD_FUNC(Soloud_deinit);
            SL_LOAD_FUNC(Soloud_getVersion);
            SL_LOAD_FUNC(Soloud_getErrorString);
            SL_LOAD_FUNC(Soloud_getBackendId);
            SL_LOAD_FUNC(Soloud_getBackendString);
            SL_LOAD_FUNC(Soloud_getBackendChannels);
            SL_LOAD_FUNC(Soloud_getBackendSamplerate);
            SL_LOAD_FUNC(Soloud_getBackendBufferSize);
            SL_LOAD_FUNC(Soloud_playEx);
            SL_LOAD_FUNC(Soloud_playBackgroundEx);
            SL_LOAD_FUNC(Soloud_seek);
            SL_LOAD_FUNC(Soloud_stop);
            SL_LOAD_FUNC(Soloud_stopAll);
            SL_LOAD_FUNC(Soloud_countAudioSource);
            SL_LOAD_FUNC(Soloud_getStreamTime);
            SL_LOAD_FUNC(Soloud_getStreamPosition);
            SL_LOAD_FUNC(Soloud_getPause);
            SL_LOAD_FUNC(Soloud_setPause);
            SL_LOAD_FUNC(Soloud_getVolume);
            SL_LOAD_FUNC(Soloud_setRelativePlaySpeed);
            SL_LOAD_FUNC(Soloud_setVolume);
            SL_LOAD_FUNC(Soloud_fadeVolume);
            SL_LOAD_FUNC(Soloud_schedulePause);
            SL_LOAD_FUNC(Soloud_scheduleStop);
            SL_LOAD_FUNC(WavStream_destroy);
            SL_LOAD_FUNC(WavStream_create);
            SL_LOAD_FUNC(WavStream_load);
            SL_LOAD_FUNC(WavStream_getLength);
            SL_LOAD_FUNC(WavStream_setVolume);
            SL_LOAD_FUNC(WavStream_stop);
            sys = sl.Soloud_create();
            if (!sys) {
                TF_ERROR(<< "Failed to create soloud instance");
                SDL_UnloadObject(sl.handle);
                return;
            }
            TF_INFO(<< "SoLoud successfully created");
            inited = true;
        }

        bool dev_open() {
            // TODO: conf
            int ret;
            if (SL_HAS_ERROR(ret = sl.Soloud_initEx(sys, SOLOUD_CLIP_ROUNDOFF, SOLOUD_AUTO, SOLOUD_AUTO, SOLOUD_AUTO, 2))) {
                TF_ERROR(<< "Failed to init SoLoud (" << SL_ERROR() << ")");
                return false;
            }
            TF_INFO(<< "SoLoud device opened (" << tf::nfstr(sl.Soloud_getBackendString(sys)) << ", " << sl.Soloud_getBackendSamplerate(sys)
            << "Hz freq, " << sl.Soloud_getBackendChannels(sys) << " channels, " << sl.Soloud_getBackendBufferSize(sys) << " chunksize)");
            dev_opened = true;
            return true;
        }

        void dev_close() {
            sl.Soloud_deinit(sys);
            dev_opened = false;
        }

        void dev_fill_arr(tf::vec<tf::str>& arr) {
            arr.push_back("Default");
            // SoLoud doesn't support that
        }

        void force_play_cache() {
            if (cache.size() == 0)
                return;
            bool from_rep = false;
            int ret;
            if (cache[0] == cur_mus) {
                if (stopped) {
                    cur_mus = nullptr;
                    force_play_cache();
                    pl::fill_cache();
                    return;
                }
                cache.erase(cache.begin());
                if (paused || sl.Soloud_countAudioSource(sys, cur_wav) > 0) {
                    paused = false;
                    update_volume();
                    if (SL_HAS_ERROR(ret = sl.Soloud_seek(sys, ch, 0.0)))
                        TF_WARN(<< "Failed to seek sound to start (" << SL_ERROR() << ")");
                    sl.Soloud_setPause(sys, ch, 0);
                    pl::fill_cache();
                    return;
                }
                from_rep = true;
            }
            stopped = false;
            if (cur_mus && sl.Soloud_countAudioSource(sys, cur_wav) > 0) {
                stopped = false;
                paused = false;
                sl.Soloud_fadeVolume(sys, ch, 0.f, (double)fade_next_time);
                sl.Soloud_scheduleStop(sys, ch, (double)fade_next_time);
                return;
            }
            Music* prev = nullptr;
            if (!from_rep) {
                prev = cur_mus;
                cur_mus = cache[0];
                cache.erase(cache.begin());
            }
            cur_mus->cached = false;
            pl::mus_open_file(cur_mus);
            stopped = false;
            paused = false;
            ch = sl.Soloud_playBackgroundEx(sys, cur_wav, volume, 0, 0);
            if (prev && prev != cur_mus && std::find(cache.begin(), cache.end(), prev) == cache.end())
                mus_close(prev);
            pl::fill_cache();
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            int ret;
            mus->h1 = sl.WavStream_create();
            if (!mus->h1) {
                TF_ERROR(<< "Failed to create wavstream");
                return false;
            }
            // Will that softlock??
            if (SL_HAS_ERROR(ret = sl.WavStream_load(mus_wav, fp))) {
                TF_ERROR(<< "Failed to load music (" << SL_ERROR() << ")");
                sl.WavStream_destroy(mus_wav);
                mus->h1 = nullptr;
                return false;
            }
            return true;
        }

        void mus_close(Music* mus) {
            if (!mus->h1)
                return;
            sl.WavStream_destroy(mus_wav);
            mus->h1 = nullptr;
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
            sl.Soloud_destroy(sys);
            inited = false;
            SDL_UnloadObject(sl.handle);
        }
    };
}

audio::AudioBase* audio::create_soloud() {
    return (audio::AudioBase*)tf::nw<audio::AudioSoLoud>();
}
#endif
