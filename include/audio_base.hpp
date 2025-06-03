#pragma once

namespace audio {
    class AudioBase {
        public:
        AudioBase();
        virtual ~AudioBase();
    };

    extern AudioBase* au;

    AudioBase* create_base();
    AudioBase* create_sdl2_mixer();
    void free_audio(AudioBase* handle);
}
