#include <audio_base.hpp>
#include <new.hpp>

using audio::AudioBase;
using audio::Music;

AudioBase* audio::au;

AudioBase::AudioBase() {
    inited = false;
    dev_opened = false;
    fade_stop_time = fade_pause_time = fade_resume_time = 0.f;
    volume = 1.f;
    max_volume = 1.f;
    cur_mus = nullptr;
    // TODO: change it
    volume = 0.05f;
    fade_resume_time = 0.2f;
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

void AudioBase::force_play(Music* mus, float pos) {

}

float AudioBase::cur_mus_get_dur() {
    if (!cur_mus)
        return 1.f;
    return cur_mus->dur;
}

float AudioBase::cur_mus_get_pos() {
    return 0.f;
}

void AudioBase::cur_mus_set_pos(float pos) {

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
