#pragma once

namespace audio {
    class AudioBase {
        public:
        bool inited;
        AudioBase();
        virtual ~AudioBase();
    };

    extern AudioBase* au;

    AudioBase* create_base();
    AudioBase* create_sdl2_mixer(bool use_mixer_x);
    void free_audio(AudioBase* handle);
}
