#pragma once
#include <music.hpp>

namespace audio {
    class AudioBase {
        public:
        bool inited;
        bool dev_opened;
        bool keep_mus_opened_when_adding;
        bool keep_mus_opened_when_played;
        AudioBase();
        virtual ~AudioBase();
        virtual bool dev_open();
        virtual void dev_close();
        virtual void update();
        virtual bool mus_open_fp(Music* mus, const char* fp);
        virtual void mus_close(Music* mus);
        virtual bool mus_opened(Music* mus);
        virtual bool mus_fill_info(Music* mus);
    };

    extern AudioBase* au;

    AudioBase* create_base();
    AudioBase* create_sdl2_mixer(bool use_mixer_x);
    void free_audio(AudioBase* handle);
}
