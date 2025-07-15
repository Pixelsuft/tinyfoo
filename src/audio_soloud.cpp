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
    unsigned int backend_from_str(const tf::str& type) {
        if (type == "auto" || type == "default")
            return SOLOUD_AUTO;
        else if (type == "sdl1")
            return SOLOUD_SDL1;
        else if (type == "sdl2")
            return SOLOUD_SDL2;
        else if (type == "portaudio")
            return SOLOUD_PORTAUDIO;
        else if (type == "winmm")
            return SOLOUD_WINMM;
        else if (type == "xaudio2")
            return SOLOUD_XAUDIO2;
        else if (type == "wasapi")
            return SOLOUD_WASAPI;
        else if (type == "alsa")
            return SOLOUD_ALSA;
        else if (type == "jack")
            return SOLOUD_JACK;
        else if (type == "oss")
            return SOLOUD_OSS;
        else if (type == "openal")
            return SOLOUD_OPENAL;
        else if (type == "coreaudio")
            return SOLOUD_COREAUDIO;
        else if (type == "opensles")
            return SOLOUD_OPENSLES;
        else if (type == "vita")
            return SOLOUD_VITA_HOMEBREW;
        else if (type == "miniaudio")
            return SOLOUD_MINIAUDIO;
        else if (type == "nosound")
            return SOLOUD_NOSOUND;
        else if (type == "null")
            return SOLOUD_NULLDRIVER;
        return SOLOUD_AUTO;
    }

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
        bool stopped;
        bool paused;

        AudioSoLoud() : AudioBase() {
            lib_name = "SoLoud";
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
            // It is supported as a backend for SoLoud
            tf::str sdl_drv_hint = conf::read_str("sdl2_mixer", "driver", "default");
            if (sdl_drv_hint.size() == 0 || sdl_drv_hint == "default")
                SDL_ResetHint(SDL_HINT_AUDIO_DRIVER);
            else if (!SDL_SetHint(SDL_HINT_AUDIO_DRIVER, sdl_drv_hint.c_str()))
                TF_WARN(<< "Failed to set SDL3 audio hint (" << SDL_GetError() << ")");
            TF_INFO(<< "SoLoud successfully created");
            inited = true;
        }

        bool dev_open() {
            tf::str need_bk = conf::read_str("soloud", "driver", "auto");
            unsigned int bk_int = SOLOUD_AUTO;
            if (need_bk.size() > 0)
                bk_int = backend_from_str(need_bk);
            int b_channels = conf::read_int("soloud", "channel", 0);
            int b_freq = conf::read_int("soloud", "frequency", 0);
            int b_samples = conf::read_int("soloud", "chunksize", 0);
            if (b_channels <= 0)
                b_channels = 2;
            if (b_freq <= 0)
                b_freq = SOLOUD_AUTO;
            if (b_samples <= 0)
                b_samples = SOLOUD_AUTO;
            int ret;
            if (SL_HAS_ERROR(ret = sl.Soloud_initEx(sys, SOLOUD_CLIP_ROUNDOFF, bk_int, (unsigned int)b_freq, (unsigned int)b_samples, (unsigned int)b_channels))) {
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
            mus->dur = (float)sl.WavStream_getLength(mus_wav);
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
            if (cur_mus && sl.Soloud_countAudioSource(sys, cur_wav) == 0) {
                ch = 0;
                if (!stopped)
                    force_play_cache();
                pl::fill_cache();
                pre_open();
            }
        }

        void cur_stop() {
            if (!cur_mus)
                return;
            bool was_paused = cur_paused();
            paused = false;
            stopped = true;
            if (was_paused)
                sl.Soloud_stop(sys, ch);
            else {
                sl.Soloud_fadeVolume(sys, ch, 0.f, (double)fade_stop_time);
                sl.Soloud_scheduleStop(sys, ch, (double)fade_stop_time);
            }
            pl::fill_cache();
        }

        float cur_get_pos() {
            if (cur_paused())
                return pause_pos;
            if (!cur_mus || stopped || sl.Soloud_countAudioSource(sys, cur_wav) == 0)
                return 0.f;
            return (float)sl.Soloud_getStreamPosition(sys, ch);
        }

        void cur_set_pos(float pos) {
            if (!cur_mus)
                return;
            pos = tf::clamp(pos, 0.f, cur_mus->dur);
            if (paused) {
                pause_pos = pos;
                return;
            }
            int ret;
            // TODO: FIXME it's broken
            if (SL_HAS_ERROR(ret = sl.Soloud_seek(sys, ch, (double)pos)))
                TF_WARN(<< "Failed to seek music (" << SL_ERROR() << ")");
        }

        void cur_pause() {
            if (paused || !cur_mus)
                return;
            pause_pos = cur_get_pos();
            paused = true;
            sl.Soloud_fadeVolume(sys, ch, 0.f, (double)fade_pause_time);
            sl.Soloud_schedulePause(sys, ch, (double)fade_pause_time);
        }

        void cur_resume() {
            if (!paused || !cur_mus)
                return;
            paused = false;
            stopped = false;
            sl.Soloud_seek(sys, ch, (double)pause_pos);
            sl.Soloud_setPause(sys, ch, 0);
            sl.Soloud_fadeVolume(sys, ch, volume, (double)fade_resume_time);
        }

        bool cur_stopped() {
            return !paused && (!cur_mus || sl.Soloud_countAudioSource(sys, cur_wav) == 0);
        }

        bool cur_paused() {
            return paused;
        }

        void update_volume() {
            volume = tf::clamp(volume, 0.f, std::min(max_volume, 1.f));
            if (cur_mus)
                sl.Soloud_setVolume(sys, ch, volume);
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
