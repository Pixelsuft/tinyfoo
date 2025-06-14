#include <audio_base.hpp>
#include <new.hpp>

using audio::AudioBase;
using audio::Music;

AudioBase* audio::au;

AudioBase::AudioBase() {
    inited = false;
    dev_opened = false;
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

bool AudioBase::mus_open_fp(Music& mus, const char* fp) {
    mus.h1 = mus.h2 = nullptr;
    mus.dur = -1.f;
    return false;
}

void AudioBase::mus_close(Music& mus) {
    
}

bool AudioBase::mus_fill_info(Music& mus) {
    return false;
}

AudioBase* audio::create_base() {
    return tf::nw<AudioBase>();
}

void audio::free_audio(AudioBase* handle) {
    tf::dl(handle);
}
