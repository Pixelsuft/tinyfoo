#pragma once

namespace audio {
    class AudioBase {
        public:
        bool inited;
        bool dev_opened;
        AudioBase();
        virtual ~AudioBase();
        virtual bool dev_open();
        virtual void dev_close();
    };

    extern AudioBase* au;

    AudioBase* create_base();
    AudioBase* create_sdl2_mixer(bool use_mixer_x);
    void free_audio(AudioBase* handle);
}
