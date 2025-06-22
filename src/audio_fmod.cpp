#include <lbs.hpp>
#if ENABLE_FMOD
#include <audio_base.hpp>
#include <new.hpp>
#include <log.hpp>
#include <break.hpp>
#include <playlist.hpp>
#include <algorithm>
#include <SDL3/SDL.h>

namespace audio {
    class AudioFMOD : public AudioBase {
        protected:
        public:
        AudioFMOD() : AudioBase() {
            lib_name = "FMOD";
        }

        ~AudioFMOD() {

        }
    };
}

audio::AudioBase* audio::create_fmod() {
    return (audio::AudioBase*)tf::nw<audio::AudioFMOD>();
}
#endif
