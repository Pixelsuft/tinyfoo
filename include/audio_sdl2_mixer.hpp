#pragma once
#include <audio_base.hpp>

namespace audio {
    class AudioSDL2Mixer : public AudioBase {
        public:
        AudioSDL2Mixer();
        ~AudioSDL2Mixer();
    };
}
