#include <audio_base.hpp>
#include <new.hpp>

using audio::AudioBase;

AudioBase* audio::au;

AudioBase::AudioBase() {

}

AudioBase::~AudioBase() {

}

AudioBase* audio::create_base() {
    return tf::bump_nw<AudioBase>();
}

void audio::free_audio(AudioBase* handle) {
    tf::bump_dl(handle);
}
