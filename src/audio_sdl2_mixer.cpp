#include <audio_sdl2_mixer.hpp>
#include <new.hpp>
#include <log.hpp>

using audio::AudioSDL2Mixer;

AudioSDL2Mixer::AudioSDL2Mixer() {
    TF_INFO(<< "TODO: SDL2_mixer support!");
}

AudioSDL2Mixer::~AudioSDL2Mixer() {
}

audio::AudioBase* audio::create_sdl2_mixer() {
    return (audio::AudioBase*)tf::bump_nw<AudioSDL2Mixer>();
}
