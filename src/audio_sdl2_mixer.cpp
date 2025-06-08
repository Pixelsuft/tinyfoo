#include <audio_base.hpp>
#include <new.hpp>
#include <log.hpp>
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
    MUS_GME
} Mix_MusicType;

typedef struct _Mix_Music Mix_Music;

#ifndef MIX_CHANNELS
#define MIX_CHANNELS    8
#endif

#define MIX_DEFAULT_FREQUENCY   44100
#define MIX_DEFAULT_FORMAT      AUDIO_S16SYS
#define MIX_DEFAULT_CHANNELS    2
#define MIX_MAX_VOLUME          SDL_MIX_MAXVOLUME

#define SDL_AUDIO_ALLOW_FREQUENCY_CHANGE    0x00000001
#define SDL_AUDIO_ALLOW_FORMAT_CHANGE       0x00000002
#define SDL_AUDIO_ALLOW_CHANNELS_CHANGE     0x00000004
#define SDL_AUDIO_ALLOW_SAMPLES_CHANGE      0x00000008
#define SDL_AUDIO_ALLOW_ANY_CHANGE          (SDL_AUDIO_ALLOW_FREQUENCY_CHANGE|SDL_AUDIO_ALLOW_FORMAT_CHANGE|SDL_AUDIO_ALLOW_CHANNELS_CHANGE|SDL_AUDIO_ALLOW_SAMPLES_CHANGE)

#define MIX_LOAD_FUNC(func_name) do { \
    *(void**)&mix.func_name = (void*)SDL_LoadFunction(mix.handle, #func_name); \
    if (!mix.func_name) { \
        TF_ERROR(<< "Failed to load SDL2_mixer" << (use_mixer_x ? "_ext" : "") << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(mix.handle); \
        return; \
    } \
} while (0)
#endif

namespace audio {
    struct SDL2MixerApi {
        SDL_SharedObject* handle;
        int (SDLCALL *Mix_Init)(int);
        void (SDLCALL *Mix_Quit)(void);
        int (SDLCALL *Mix_OpenAudioDevice)(int, uint16_t, int, int, const char*, int);
        int (SDLCALL *Mix_QuerySpec)(int*, uint16_t*, int*);
        int (SDLCALL *Mix_AllocateChannels)(int);
        void (SDLCALL *Mix_FreeMusic)(Mix_Music*);
        void (SDLCALL *Mix_HookMusic)(void (SDLCALL *)(void*, uint8_t*, int), void*);
        void (SDLCALL *Mix_HookMusicFinished)(void (SDLCALL *)(void));
        void* (SDLCALL *Mix_GetMusicHookData)(void);
        void (SDLCALL *Mix_ChannelFinished)(void (SDLCALL *)(int));
        int (SDLCALL *Mix_PlayMusic)(Mix_Music*, int);
        int (SDLCALL *Mix_VolumeMusic)(int);
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
        public:
        AudioSDL2Mixer(bool use_mixer_x) {
            const char* lib_name = IS_WIN ? (use_mixer_x ? "SDL2_mixer_ext.dll" : "SDL2_mixer.dll") : (use_mixer_x ? "libSDL2_mixer_ext.so" : "libSDL2_mixer.so");
            mix.handle = SDL_LoadObject(lib_name);
            if (!mix.handle) {
                TF_WARN(<< "Failed to load SDL2_mixer" << (use_mixer_x ? "_ext" : "") << " library (" << SDL_GetError() << ")");
                return;
            }
            // TODO: error if old version
            MIX_LOAD_FUNC(Mix_Init);
            MIX_LOAD_FUNC(Mix_Quit);
            MIX_LOAD_FUNC(Mix_OpenAudioDevice);
            MIX_LOAD_FUNC(Mix_QuerySpec);
            MIX_LOAD_FUNC(Mix_AllocateChannels);
            MIX_LOAD_FUNC(Mix_FreeMusic);
            MIX_LOAD_FUNC(Mix_HookMusic);
            MIX_LOAD_FUNC(Mix_HookMusicFinished);
            MIX_LOAD_FUNC(Mix_GetMusicHookData);
            MIX_LOAD_FUNC(Mix_ChannelFinished);
            MIX_LOAD_FUNC(Mix_PlayMusic);
            MIX_LOAD_FUNC(Mix_VolumeMusic);
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
            int init_flags = MIX_INIT_MP3; // TODO
            int ret_flags = mix.Mix_Init(init_flags);
            if (ret_flags == 0) {
                TF_ERROR(<< "Failed to init SDL2_mixer" << (use_mixer_x ? "_ext" : "") << " (" << SDL_GetError() << ")");
                SDL_UnloadObject(mix.handle);
                return;
            }
            if (ret_flags < init_flags)
                TF_WARN(<< "Failed to init some SDL2_mixer" << (use_mixer_x ? "_ext" : "") << "formats (" << SDL_GetError() << ")");
            TF_INFO(<< "SDL2_mixer" << (use_mixer_x ? "_ext" : "") << " inited successfully");
            inited = true;
        }

        bool dev_open() {
            // TODO: configure
            if (mix.Mix_OpenAudioDevice(48000, SDL_AUDIO_F32, 2, 2048, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE) < 0) {
                TF_ERROR(<< "Failed to open audio device (" << SDL_GetError() << ")");
                return false;
            }
            int num_fr = 0;
            Uint16 num_fmt = 0;
            int num_ch = 0;
            mix.Mix_QuerySpec(&num_fr, &num_fmt, &num_ch);
            TF_INFO(<< "Audio device opened (" << num_fr << "Hz freq, " << num_ch << " channels)");
            dev_opened = true;
            return true;
        }

        void dev_close() {
            mix.Mix_CloseAudio();
            dev_opened = false;
        }

        ~AudioSDL2Mixer() {
            if (!inited)
                return;
            if (dev_opened)
                dev_close();
            inited = false;
            mix.Mix_Quit();
            SDL_UnloadObject(mix.handle);
        }
    };
}

audio::AudioBase* audio::create_sdl2_mixer(bool use_mixer_x) {
    return (audio::AudioBase*)tf::nw<audio::AudioSDL2Mixer>(use_mixer_x);
}
