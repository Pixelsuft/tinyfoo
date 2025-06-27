#include <audio_base.hpp>
#include <new.hpp>
#include <playlist.hpp>
#include <conf.hpp>
#include <algorithm>

using audio::AudioBase;
using audio::Music;

AudioBase* audio::au;

AudioBase::AudioBase() {
    lib_name = "Dummy";
    cache.reserve(256); // Why not lul
    inited = false;
    dev_opened = false;
    fade_stop_time = fade_pause_time = fade_resume_time = 0.f;
    volume = 1.f;
    max_volume = 1.f;
    cur_mus = nullptr;
    cache_opened_cnt = 8;
    // TODO: change it
    volume = 0.05f;
    fade_resume_time = 0.2f;
    fade_pause_time = 0.2f;
    fade_stop_time = 0.2f;
    fade_next_time = 0.2f;
    if (conf::get().contains("audio") && conf::get().at("audio").is_table()) {
        toml::value tab = conf::get().at("audio");
        need_dev = toml::find_or<tf::str>(tab, "device", need_dev);
    }
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
    // mus->dur = -1.f;
    return false;
}

void AudioBase::update() {

}

void AudioBase::mus_close(Music* mus) {
    
}

void AudioBase::update_volume() {

}

void AudioBase::force_play_cache() {

}

void AudioBase::cur_stop() {

}

float AudioBase::cur_get_dur() {
    if (!cur_mus)
        return 1.f;
    return cur_mus->dur;
}

float AudioBase::cur_get_pos() {
    return 0.f;
}

void AudioBase::cur_set_pos(float pos) {
    (void)pos;
}

void AudioBase::cur_pause() {
    
}

void AudioBase::cur_resume() {
    
}

bool AudioBase::cur_paused() {
    return false;
}

bool AudioBase::cur_stopped() {
    return false;
}

bool AudioBase::mus_fill_info(Music* mus) {
    return false;
}

bool AudioBase::mus_opened(Music* mus) {
    return mus->h1 != nullptr;
}

bool AudioBase::mus_beging_used(Music* mus) {
    if (cur_mus == mus)
        return true;
    return std::find(cache.begin(), cache.end(), mus) != cache.end();
}

void AudioBase::pre_open() {
    pl::fill_cache();
    int cnt = std::min((int)cache.size(), cache_opened_cnt);
    for (int i = 0; i < cnt; i++)
        pl::mus_open_file(cache[i]);
}

AudioBase* audio::create_base() {
    return tf::nw<AudioBase>();
}

void audio::free_audio(AudioBase* handle) {
    tf::dl(handle);
}
