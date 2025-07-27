#include <audio.hpp>
#include <new.hpp>
#include <playlist.hpp>
#include <conf.hpp>
#include <SDL3/SDL_stdinc.h>
#include <algorithm>

using audio::AudioBase;
using audio::Music;

AudioBase* audio::au;

AudioBase::AudioBase() {
    lib_name = "Dummy";
    cache.reserve(256); // Why not lul
    inited = false;
    dev_opened = false;
    cur_mus = nullptr;
    cache_opened_cnt = 8;
    temp_cache_cnt = 0;
    order_mode = conf::read_int("playback", "order", 1);
    if (order_mode == 3 && !ORDER_COOL_RNG_PATCH)
        order_mode = 2;
    repeat_blocks = conf::read_int("audio", "repeat_blocks", 0);
    need_dev = conf::read_str("audio", "device", need_dev);
    max_volume = conf::read_float("audio", "max_volume", 100.f) / 100.f;
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
    (void)fp;
    mus->h1 = mus->h2 = nullptr;
    // mus->dur = -1.f;
    return false;
}

void AudioBase::update() {

}

void AudioBase::mus_close(Music* mus) {
    (void)mus;
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
    return true;
}

bool AudioBase::mus_fill_info(Music* mus) {
    (void)mus;
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

void audio::mus_fill_format_default(Music* mus) {
    mus->type = Type::NONE;
    // Hacky
    char ext_buf[5];
    if (mus->full_path.size() >= 5) {
        ext_buf[0] = SDL_tolower(mus->full_path[mus->full_path.size() - 5]);
        ext_buf[1] = SDL_tolower(mus->full_path[mus->full_path.size() - 4]);
        ext_buf[2] = SDL_tolower(mus->full_path[mus->full_path.size() - 3]);
        ext_buf[3] = SDL_tolower(mus->full_path[mus->full_path.size() - 2]);
        ext_buf[4] = SDL_tolower(mus->full_path[mus->full_path.size() - 1]);
    }
    else
        ext_buf[0] = ext_buf[1] = ext_buf[2] = ext_buf[3] = ext_buf[4] = '\0';
    if (SDL_memcmp(ext_buf + 1, ".mp3", 4) == 0)
        mus->type = Type::MP3;
    else if (SDL_memcmp(ext_buf + 1, ".ogg", 4) == 0)
        mus->type = Type::OGG;
    else if (SDL_memcmp(ext_buf + 1, ".wav", 4) == 0)
        mus->type = Type::WAV;
    else if (SDL_memcmp(ext_buf, ".flac", 5) == 0)
        mus->type = Type::FLAC;
    else if (SDL_memcmp(ext_buf, ".midi", 5) == 0 || SDL_memcmp(ext_buf + 1, ".mid", 4) == 0)
        mus->type = Type::MID;
}
