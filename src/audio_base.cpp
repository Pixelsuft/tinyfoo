#include <audio_base.hpp>
#include <new.hpp>

using audio::AudioBase;

AudioBase* audio::au;

AudioBase::AudioBase() {

}

AudioBase::~AudioBase() {

}

AudioBase* audio::create_base() {
    return tf::nw<AudioBase>();
}

void audio::free_audio(AudioBase* handle) {
    tf::dl(handle);
}
