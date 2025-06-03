#pragma once
#include <audio_base.hpp>

namespace audio {
    class AudioSDL2Mixer : public AudioBase {
        public:
        AudioSDL2Mixer(bool use_mixer_x);
        ~AudioSDL2Mixer();
    };
}
