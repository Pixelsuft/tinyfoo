#include <audio_base.hpp>
#include <new.hpp>
#include <log.hpp>

namespace audio {
    class AudioSDL2Mixer : public AudioBase {
        public:
        AudioSDL2Mixer(bool use_mixer_x) {
            TF_INFO(<< "TODO: SDL2_mixer support!");
        }

        ~AudioSDL2Mixer() {

        }
    };
}

audio::AudioBase* audio::create_sdl2_mixer(bool use_mixer_x) {
    return (audio::AudioBase*)tf::nw<audio::AudioSDL2Mixer>(use_mixer_x);
}
