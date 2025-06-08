#include <audio_base.hpp>
#include <new.hpp>

using audio::AudioBase;

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

AudioBase* audio::create_base() {
    return tf::nw<AudioBase>();
}

void audio::free_audio(AudioBase* handle) {
    tf::dl(handle);
}
