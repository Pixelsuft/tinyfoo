#include <audio_base.hpp>
#include <new.hpp>

using audio::AudioBase;
using audio::Music;

AudioBase* audio::au;

AudioBase::AudioBase() {
    inited = false;
    dev_opened = false;
    keep_mus_opened_when_adding = false;
    keep_mus_opened_when_played = false;
    fade_stop_time = fade_pause_time = fade_resume_time = 0.f;
    volume = 0.1f; // TODO: change it
    max_volume = 1.f;
}

AudioBase::~AudioBase() {
    dev_opened = false;
    inited = false;
}

bool AudioBase::dev_open() {
    dev_opened = true;
    return true;
}

void AudioBase::dev_close() {
    dev_opened = false;
}

bool AudioBase::mus_open_fp(Music* mus, const char* fp) {
    mus->h1 = mus->h2 = nullptr;
    mus->dur = -1.f;
    return false;
}

void AudioBase::update() {

}

void AudioBase::mus_close(Music* mus) {
    
}

void AudioBase::update_volume() {

}

bool AudioBase::mus_fill_info(Music* mus) {
    return false;
}

bool AudioBase::mus_opened(Music* mus) {
    return mus->h1 != nullptr;
}

AudioBase* audio::create_base() {
    return tf::nw<AudioBase>();
}

void audio::free_audio(AudioBase* handle) {
    tf::dl(handle);
}
