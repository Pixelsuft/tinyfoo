#include <lbs.hpp>
#if ENABLE_SDL2_MIXER
#include <audio_base.hpp>
#include <new.hpp>
#include <log.hpp>
#include <break.hpp>
#include <playlist.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <SDL3/SDL.h>
#if 1
typedef enum {
    MIX_INIT_FLAC = 0x00000001,
    MIX_INIT_MOD = 0x00000002,
    MIX_INIT_MP3 = 0x00000008,
    MIX_INIT_OGG = 0x00000010,
    MIX_INIT_MID = 0x00000020,
    MIX_INIT_OPUS = 0x00000040,
    MIX_INIT_WAVPACK = 0x00000080
} MIX_InitFlags;

typedef enum {
    MUS_NONE,
    MUS_CMD,
    MUS_WAV,
    MUS_MOD,
    MUS_MID,
    MUS_OGG,
    MUS_MP3,
    MUS_MP3_MAD_UNUSED,
    MUS_FLAC,
    MUS_MODPLUG_UNUSED,
    MUS_OPUS,
    MUS_WAVPACK,
    MUS_GME,
    MUS_FFMPEG = 100,
    MUS_PXTONE,
    MUS_ADLMIDI = 200,
    MUS_OPNMIDI,
    MUS_FLUIDLITE,
    MUS_EDMIDI,
    MUS_NATIVEMIDI
} Mix_MusicType;

typedef struct _Mix_Music Mix_Music;

#ifndef MIX_CHANNELS
#define MIX_CHANNELS    8
#endif

#define MIX_MAX_VOLUME          128

#define SDL_AUDIO_ALLOW_FREQUENCY_CHANGE    0x00000001
#define SDL_AUDIO_ALLOW_FORMAT_CHANGE       0x00000002
#define SDL_AUDIO_ALLOW_CHANNELS_CHANGE     0x00000004
#define SDL_AUDIO_ALLOW_SAMPLES_CHANGE      0x00000008
#define SDL_AUDIO_ALLOW_ANY_CHANGE          (SDL_AUDIO_ALLOW_FREQUENCY_CHANGE|SDL_AUDIO_ALLOW_FORMAT_CHANGE|SDL_AUDIO_ALLOW_CHANNELS_CHANGE|SDL_AUDIO_ALLOW_SAMPLES_CHANGE)

#define MIX_LOAD_FUNC(func_name) do { \
    *(void**)&mix.func_name = (void*)SDL_LoadFunction(mix.handle, #func_name); \
    if (!mix.func_name) { \
        TF_ERROR(<< "Failed to load " << lib_name << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(mix.handle); \
        return; \
    } \
} while (0)
#endif
#define mus_h ((Mix_Music*)mus->h1)
#define cur_h ((Mix_Music*)cur_mus->h1)

void SDLCALL sdl2_music_finish_cb(void);

namespace audio {
    SDL_AudioFormat fmt_from_str(const tf::str& fmt) {
        if (fmt == "SDL_AUDIO_U8")
            return SDL_AUDIO_U8;
        else if (fmt == "SDL_AUDIO_S8")
            return SDL_AUDIO_S8;
        else if (fmt == "SDL_AUDIO_S16LE")
            return SDL_AUDIO_S16LE;
        else if (fmt == "SDL_AUDIO_S16BE")
            return SDL_AUDIO_S16BE;
        else if (fmt == "SDL_AUDIO_S32LE")
            return SDL_AUDIO_S32LE;
        else if (fmt == "SDL_AUDIO_S32BE")
            return SDL_AUDIO_S32BE;
        else if (fmt == "SDL_AUDIO_F32LE")
            return SDL_AUDIO_F32LE;
        else if (fmt == "SDL_AUDIO_F32BE")
            return SDL_AUDIO_F32BE;
        else if (fmt == "SDL_AUDIO_S16")
            return SDL_AUDIO_S16;
        else if (fmt == "SDL_AUDIO_S32")
            return SDL_AUDIO_S32;
        else if (fmt == "SDL_AUDIO_F32")
            return SDL_AUDIO_F32;
        TF_WARN(<< "Unknown audio format, using SDL_AUDIO_S16");
        return SDL_AUDIO_S16;
    }

    const char* fmt_to_str(SDL_AudioFormat fmt) {
        return SDL_GetAudioFormatName(fmt);
    }

    struct SDL2MixerApi {
        SDL_SharedObject* handle;
        int (SDLCALL *Mix_Init)(int);
        void (SDLCALL *Mix_Quit)(void);
        int (SDLCALL *Mix_OpenAudioDevice)(int, uint16_t, int, int, const char*, int);
        int (SDLCALL *Mix_QuerySpec)(int*, uint16_t*, int*);
        int (SDLCALL *Mix_AllocateChannels)(int);
        Mix_Music* (SDLCALL *Mix_LoadMUS)(const char*);
        Mix_MusicType(SDLCALL *Mix_GetMusicType)(const Mix_Music*);
        void (SDLCALL *Mix_FreeMusic)(Mix_Music*);
        void (SDLCALL *Mix_HookMusic)(void (SDLCALL *)(void*, uint8_t*, int), void*);
        void (SDLCALL *Mix_HookMusicFinished)(void (SDLCALL *)(void));
        void* (SDLCALL *Mix_GetMusicHookData)(void);
        void (SDLCALL *Mix_ChannelFinished)(void (SDLCALL *)(int));
        int (SDLCALL *Mix_FadeInMusicPos)(Mix_Music*, int, int, double);
        int (SDLCALL *Mix_VolumeMusic)(int);
        int (SDLCALL *Mix_FadeOutMusic)(int);
        int (SDLCALL *Mix_HaltMusic)(void);
        void (SDLCALL *Mix_PauseMusic)(void);
        void (SDLCALL *Mix_ResumeMusic)(void);
        void (SDLCALL *Mix_RewindMusic)(void);
        int (SDLCALL *Mix_PausedMusic)(void);
        int (SDLCALL *Mix_SetMusicPosition)(double);
        double (SDLCALL *Mix_GetMusicPosition)(Mix_Music*);
        double (SDLCALL *Mix_MusicDuration)(Mix_Music*);
        int (SDLCALL *Mix_PlayingMusic)(void);
        void (SDLCALL *Mix_CloseAudio)(void);
    };

    class AudioSDL2Mixer : public AudioBase {
        protected:
        SDL2MixerApi mix;
        float pause_pos;
        public:
        bool was_finished;
        bool hooked;
        bool stopped;
        bool paused;

        AudioSDL2Mixer(bool use_mixer_x) : AudioBase() {
            lib_name = use_mixer_x ? "SDL2_mixer_ext" : "SDL2_mixer";
            was_finished = false;
            hooked = false;
            stopped = false;
            paused = false;
            pause_pos = 0.f;
            const char* lib_path = IS_WIN ? (use_mixer_x ? "SDL2_mixer_ext.dll" : "SDL2_mixer.dll") : (use_mixer_x ? "libSDL2_mixer_ext.so" : "libSDL2_mixer.so");
            mix.handle = SDL_LoadObject(lib_path);
            if (!mix.handle) {
                TF_WARN(<< "Failed to load " << lib_name << " library (" << SDL_GetError() << ")");
                return;
            }
            // TODO: error if old version
            MIX_LOAD_FUNC(Mix_Init);
            MIX_LOAD_FUNC(Mix_Quit);
            MIX_LOAD_FUNC(Mix_OpenAudioDevice);
            MIX_LOAD_FUNC(Mix_QuerySpec);
            MIX_LOAD_FUNC(Mix_AllocateChannels);
            MIX_LOAD_FUNC(Mix_LoadMUS);
            MIX_LOAD_FUNC(Mix_GetMusicType);
            MIX_LOAD_FUNC(Mix_FreeMusic);
            MIX_LOAD_FUNC(Mix_HookMusic);
            MIX_LOAD_FUNC(Mix_HookMusicFinished);
            MIX_LOAD_FUNC(Mix_GetMusicHookData);
            MIX_LOAD_FUNC(Mix_ChannelFinished);
            MIX_LOAD_FUNC(Mix_FadeInMusicPos);
            MIX_LOAD_FUNC(Mix_VolumeMusic);
            MIX_LOAD_FUNC(Mix_FadeOutMusic);
            MIX_LOAD_FUNC(Mix_HaltMusic);
            MIX_LOAD_FUNC(Mix_PauseMusic);
            MIX_LOAD_FUNC(Mix_ResumeMusic);
            MIX_LOAD_FUNC(Mix_RewindMusic);
            MIX_LOAD_FUNC(Mix_PausedMusic);
            MIX_LOAD_FUNC(Mix_SetMusicPosition);
            MIX_LOAD_FUNC(Mix_GetMusicPosition);
            MIX_LOAD_FUNC(Mix_MusicDuration);
            MIX_LOAD_FUNC(Mix_PlayingMusic);
            MIX_LOAD_FUNC(Mix_CloseAudio);
            tf::str drv_hint;
            bool allow_flac = false;
            bool allow_mod = false;
            bool allow_mp3 = true;
            bool allow_ogg = false;
            bool allow_mid = false;
            bool allow_opus = false;
            bool allow_wavpack = false;
            if (conf::get().contains("sdl2_mixer") && conf::get().at("sdl2_mixer").is_table()) {
                toml::value tab = conf::get().at("sdl2_mixer");
                drv_hint = conf::read_str(tab, "driver", "");
                allow_flac = conf::read_bool(tab, "enable_flac", false);
                allow_mod = conf::read_bool(tab, "enable_mod", false);
                allow_mp3 = conf::read_bool(tab, "enable_mp3", true);
                allow_ogg = conf::read_bool(tab, "enable_ogg", false);
                allow_mid = conf::read_bool(tab, "enable_mid", false);
                allow_opus = conf::read_bool(tab, "enable_opus", false);
                allow_wavpack = conf::read_bool(tab, "enable_wavpack", false);
            }
            if (drv_hint.size() == 0)
                SDL_ResetHint(SDL_HINT_AUDIO_DRIVER);
            else if (!SDL_SetHint(SDL_HINT_AUDIO_DRIVER, drv_hint.c_str()))
                TF_WARN(<< "Failed to set SDL3 audio hint (" << SDL_GetError() << ")");
            if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
                TF_ERROR(<< "Failed to init SDL3 audio (" << SDL_GetError() << ")");
                SDL_UnloadObject(mix.handle);
                display_available_drivers();
                return;
            }
            int init_flags = (allow_flac ? MIX_INIT_FLAC : 0) | (allow_mod ? MIX_INIT_MOD : 0) |
                (allow_mp3 ? MIX_INIT_MP3 : 0) | (allow_ogg ? MIX_INIT_OGG : 0) | (allow_mid ? MIX_INIT_MID : 0) |
                (allow_opus ? MIX_INIT_OPUS : 0) | (allow_wavpack ? MIX_INIT_WAVPACK : 0);
            int ret_flags = mix.Mix_Init(init_flags);
            if (ret_flags == 0 && (init_flags > 0 || SDL_GetError()[0] != '\0')) {
                TF_ERROR(<< "Failed to init " << lib_name << " (" << SDL_GetError() << ")");
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                SDL_UnloadObject(mix.handle);
                return;
            }
            if (ret_flags < init_flags)
                TF_WARN(<< "Failed to init some " << lib_name << "formats (" << SDL_GetError() << ")");
            TF_INFO(<< lib_name << " inited successfully with " << tf::nfstr(SDL_GetCurrentAudioDriver()) << " driver");
            inited = true;
        }

        bool dev_open() {
            SDL_AudioDeviceID dev_id = SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;
            if (0 && conf::get().contains("sdl2_mixer") && conf::get().at("sdl2_mixer").is_table()) {
                toml::value tab = conf::get().at("sdl2_mixer");
                need_dev = conf::read_str(tab, "device", need_dev);
            }
            const char* dev_name = nullptr;
            int dev_count;
            SDL_AudioDeviceID* dev_arr = SDL_GetAudioPlaybackDevices(&dev_count);
            if (dev_arr) {
                for (int i = 0; i < dev_count; i++) {
                    const char* cur_name = SDL_GetAudioDeviceName(dev_arr[i]);
                    if (!cur_name) {
                        TF_ERROR(<< "Failed to get audio device name (" << SDL_GetError() << ")");
                        continue;
                    }
                    if (!SDL_strcmp(need_dev.c_str(), cur_name)) {
                        dev_name = cur_name;
                        dev_id = dev_arr[i];
                        break;
                    }
                    // TF_INFO(<< "Found audio device: " << cur_name);
                }
                SDL_free(dev_arr);
            }
            else
                TF_ERROR(<< "Failed to get playback devices (" << SDL_GetError() << ")");
            SDL_AudioSpec spec;
            int sample_frames;
            if (!SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, &sample_frames)) {
                TF_ERROR(<< "Failed to get output device specs (" << SDL_GetError() << ")");
                spec.channels = 2;
                spec.freq = 2;
                spec.format = SDL_AUDIO_F32;
            }
            if (conf::get().contains("sdl2_mixer") && conf::get().at("sdl2_mixer").is_table()) {
                toml::value tab = conf::get().at("sdl2_mixer");
                spec.format = fmt_from_str(conf::read_str(tab, "format", fmt_to_str(spec.format)));
                int b_channels = conf::read_int(tab, "channel", 0);
                int b_freq = conf::read_int(tab, "frequency", 0);
                int b_samples = conf::read_int(tab, "chunksize", 0);
                if (b_channels > 0)
                    spec.channels = b_channels;
                if (b_freq > 0)
                    spec.freq = b_freq;
                if (b_samples > 0)
                    sample_frames = b_samples;
            }
            // TF_INFO(<< " " << sample_frames << " " << spec.channels << " " << spec.format);
            if (mix.Mix_OpenAudioDevice(spec.freq, (uint16_t)spec.format, spec.channels, sample_frames, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE) < 0) {
                TF_ERROR(<< "Failed to open audio device (" << SDL_GetError() << ")");
                return false;
            }
            int num_fr = 0;
            Uint16 num_fmt = 0;
            int num_ch = 0;
            mix.Mix_QuerySpec(&num_fr, &num_fmt, &num_ch);
            mix.Mix_AllocateChannels(0);
            TF_INFO(<< "Audio device \"" << tf::nfstr(dev_name) << "\" opened (" << num_fr << "Hz freq, " << num_ch << " channels, "
                << fmt_to_str((SDL_AudioFormat)num_fmt) << " format, " << sample_frames << " chunksize)");
            dev_opened = true;
            return true;
        }

        void dev_close() {
            mix.Mix_CloseAudio();
            dev_opened = false;
        }

        void dev_fill_arr(tf::vec<tf::str>& arr) {
            arr.push_back("Default");
            int dev_count;
            SDL_AudioDeviceID* dev_arr = SDL_GetAudioPlaybackDevices(&dev_count);
            if (!dev_arr) {
                TF_ERROR(<< "Failed to get playback devices (" << SDL_GetError() << ")");
                return;
            }
            for (int i = 0; i < dev_count; i++) {
                const char* cur_name = SDL_GetAudioDeviceName(dev_arr[i]);
                if (!cur_name) {
                    TF_ERROR(<< "Failed to get audio device name (" << SDL_GetError() << ")");
                    continue;
                }
                arr.push_back(cur_name);
            }
            SDL_free(dev_arr);
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1) {
#if !IS_RELEASE
                // TF_WARN(<< "Music \"" << mus->fn << "\" already opened");
#endif
                return true;
            }
            Mix_Music* h = mix.Mix_LoadMUS(fp);
            mus->h1 = h;
            if (!h) {
                TF_ERROR(<< "Failed to open music \"" << fp << "\" (" << SDL_GetError() << ")");
                return false;
            }
            return true;
        }

        void mus_close(Music* mus) {
            if (!mus->h1)
                return;
            mix.Mix_FreeMusic(mus_h);
            mus->h1 = nullptr;
        }

        void force_play_cache() {
            if (cache.size() == 0)
                return;
            bool from_rep = false;
            if (cache[0] == cur_mus) {
                if (stopped || was_finished) {
                    // Hack
                    cur_mus = nullptr;
                    force_play_cache();
                    pl::fill_cache();
                    return;
                }
                cache.erase(cache.begin());
                if (mix.Mix_PlayingMusic()) {
                    cur_set_pos(0.f);
                    pl::fill_cache();
                    return;
                }
                from_rep = true;
            }
            stopped = false;
            if (hooked) {
                if (fade_next_time <= 0.f || cur_paused())
                    mix.Mix_HaltMusic();
                else
                    mix.Mix_FadeOutMusic((int)(fade_next_time * 1000.f));
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
            paused = false;
            if (mix.Mix_FadeInMusicPos(cur_h, 0, 0, 0.0) < 0) {
                hooked = false;
                was_finished = true;
                TF_WARN(<< "Failed to play music (" << SDL_GetError() << ")");
            }
            else {
                hooked = true;
                was_finished = false;
                mix.Mix_VolumeMusic((int)(volume * (float)MIX_MAX_VOLUME));
                mix.Mix_HookMusicFinished(sdl2_music_finish_cb);
            }
            if (prev && prev != cur_mus && std::find(cache.begin(), cache.end(), prev) == cache.end())
                mus_close(prev);
            pl::fill_cache();
        }
    
        bool mus_fill_info(Music* mus) {
            double dur = mix.Mix_MusicDuration(mus_h);
            if (dur < 0.0) {
                mus->dur = 0.f;
                TF_WARN(<< "Failed to get music duration (" << SDL_GetError() << ")");
                // return false;
            }
            mus->dur = (float)dur;
            switch (mix.Mix_GetMusicType(mus_h)) {
            case MUS_WAV: {
                mus->type = Type::WAV;
                break;
            }
            case MUS_MOD: {
                mus->type = Type::MOD;
                break;
            }
            case MUS_MID:
            case MUS_ADLMIDI: 
            case MUS_OPNMIDI: 
            case MUS_FLUIDLITE: 
            case MUS_EDMIDI: 
            case MUS_NATIVEMIDI: {
                mus->type = Type::MID;
                break;
            }
            case MUS_OGG: {
                mus->type = Type::OGG;
                break;
            }
            case MUS_MP3: {
                mus->type = Type::MP3;
                break;
            }
            case MUS_FLAC: {
                mus->type = Type::FLAC;
                break;
            }
            case MUS_OPUS: {
                mus->type = Type::OPUS;
                break;
            }
            case MUS_WAVPACK: {
                mus->type = Type::WAVPACK;
                break;
            }
            case MUS_GME: {
                mus->type = Type::GME;
                break;
            }
            case MUS_PXTONE: {
                mus->type = Type::PXTONE;
                break;
            }
            case MUS_FFMPEG: {
                mus->type = Type::AAC;  // Not only AAC
                break;
            }
            default: {
                mus->type = Type::NONE;
                break;
            }
            }
            return true;
        }

        void update() {
            if (was_finished) {
                was_finished = false;
                hooked = false;
                if (paused) {
                    return;
                }
                if (!stopped)
                    force_play_cache();
                pl::fill_cache();
                pre_open();
            }
        }

        void cur_stop() {
            bool was_paused = cur_paused();
            paused = false;
            stopped = true;
            if (fade_stop_time <= 0.f || was_paused)
                mix.Mix_HaltMusic();
            else
                mix.Mix_FadeOutMusic((int)(fade_stop_time * 1000.f));
            pl::fill_cache();
        }

        float cur_get_pos() {
            if (cur_paused())
                return pause_pos;
            if (!cur_mus || stopped || !mix.Mix_PlayingMusic())
                return 0.f;
            double ret = mix.Mix_GetMusicPosition(cur_h);
            if (ret < 0.0) {
                TF_WARN(<< "Failed to get current music pos (" << SDL_GetError() << ")");
                return 0.f;
            }
            return (float)ret;
        }

        void cur_set_pos(float pos) {
            if (!cur_mus || !hooked)
                return;
            pos = tf::clamp(pos, 0.f, cur_mus->dur);
            if (paused) {
                pause_pos = pos;
                return;
            }
            if (mix.Mix_SetMusicPosition((double)pos) < 0)
                TF_WARN(<< "Failed to set current music pos (" << SDL_GetError() << ")");
        }

        void cur_pause() {
            if (paused || !cur_mus)
                return;
            pause_pos = cur_get_pos();
            // Should I do that???
            // pause_pos += fade_pause_time;
            paused = true;
            if (fade_pause_time <= 0.f)
                mix.Mix_HaltMusic();
            else
                mix.Mix_FadeOutMusic((int)(fade_pause_time * 1000.f));
        }

        void cur_resume() {
            if (!paused || !cur_mus)
                return;
            paused = false;
            stopped = false;
            if (mix.Mix_FadeInMusicPos(cur_h, 0, (int)(fade_resume_time * 1000.f), (double)pause_pos) < 0) {
                hooked = false;
                was_finished = true;
                TF_WARN(<< "Failed to resume music (" << SDL_GetError() << ")");
            }
            else {
                hooked = true;
                was_finished = false;
                mix.Mix_VolumeMusic((int)(volume * (float)MIX_MAX_VOLUME));
                mix.Mix_HookMusicFinished(sdl2_music_finish_cb);
            }
        }

        bool cur_stopped() {
            return !hooked && !paused;
        }

        bool cur_paused() {
            return paused;
        }

        void update_volume() {
            volume = tf::clamp(volume, 0.f, 1.f);
            if (!mix.Mix_PlayingMusic())
                return;
            mix.Mix_VolumeMusic((int)(volume * (float)MIX_MAX_VOLUME));            
        }

        void display_available_drivers() {
            int num = SDL_GetNumAudioDrivers();
            TF_INFO(<< "Available audio drivers: ");
            for (int i = 0; i < num; i++) {
                TF_INFO(<< SDL_GetAudioDriver(i));
            }
        }

        ~AudioSDL2Mixer() {
            if (!inited)
                return;
            if (dev_opened)
                dev_close();
            inited = false;
            mix.Mix_Quit();
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
            SDL_UnloadObject(mix.handle);
        }
    };
}

void SDLCALL sdl2_music_finish_cb(void) {
    audio::AudioSDL2Mixer* a = (audio::AudioSDL2Mixer*)audio::au;
    a->was_finished = true;
    // TF_INFO(<< "Finished!");
}

audio::AudioBase* audio::create_sdl2_mixer(bool use_mixer_x) {
    return (audio::AudioBase*)tf::nw<audio::AudioSDL2Mixer>(use_mixer_x);
}
#endif
