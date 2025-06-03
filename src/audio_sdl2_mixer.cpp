#include <audio_sdl2_mixer.hpp>
#include <new.hpp>
#include <log.hpp>

using audio::AudioSDL2Mixer;

AudioSDL2Mixer::AudioSDL2Mixer(bool use_mixer_x) {
    TF_INFO(<< "TODO: SDL2_mixer support!");
}

AudioSDL2Mixer::~AudioSDL2Mixer() {
}

audio::AudioBase* audio::create_sdl2_mixer(bool use_mixer_x) {
    return (audio::AudioBase*)tf::nw<AudioSDL2Mixer>(use_mixer_x);
}
