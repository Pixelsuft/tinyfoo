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
    max_volume = 1.f; // TODO: maybe read from conf?
    cur_mus = nullptr;
    cache_opened_cnt = 8;
    order_mode = 1; // TODO: read from conf
    temp_cache_cnt = 0;
    need_dev = conf::read_str("audio", "device", need_dev);
    volume = conf::read_float("audio", "volume", 25.f) / 100.f;
    fade_next_time = conf::read_float("audio", "fade_next_time", 100.f) / 1000.f;
    fade_pause_time = conf::read_float("audio", "fade_pause_time", 100.f) / 1000.f;
    fade_stop_time = conf::read_float("audio", "fade_stop_time", 100.f) / 1000.f;
    fade_resume_time = conf::read_float("audio", "fade_resume_time", 100.f) / 1000.f;
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

void AudioBase::dev_fill_arr(tf::vec<tf::str>& arr) {
    arr.push_back("Default");
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
