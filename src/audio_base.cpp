#include <audio_base.hpp>
#include <new.hpp>

using audio::AudioBase;
using audio::MusicBase;

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

MusicBase* AudioBase::mus_from_fp(const char* fp) {
#if 1
    (void)fp;
    auto ret = tf::nw<MusicBase>();
    ret->duration = -1.f;
    return ret;
#endif
    return nullptr;
}

bool AudioBase::mus_fill_info(MusicBase* mus) {
    return false;
}

void AudioBase::mus_free(MusicBase* mus) {
    tf::bump_dl(mus);
}

AudioBase* audio::create_base() {
    return tf::nw<AudioBase>();
}

void audio::free_audio(AudioBase* handle) {
    tf::dl(handle);
}
