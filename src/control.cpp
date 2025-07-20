#include <control.hpp>
#include <audio.hpp>
#include <playlist.hpp>

void ctrl::stop() {
    audio::au->cur_stop();
}

void ctrl::play() {
    if (audio::au->cur_paused())
        audio::au->cur_resume();
    else if (audio::au->cur_stopped()) {
        pl::fill_cache();
        audio::au->force_play_cache();
    }
    else
        audio::au->cur_set_pos(0.f);
}

void ctrl::pause() {
    if (audio::au->cur_stopped()) {
        pl::fill_cache();
        audio::au->force_play_cache();
    }
    else if (audio::au->cur_paused())
        audio::au->cur_resume();
    else
        audio::au->cur_pause();
}

void ctrl::next() {
    pl::fill_cache();
    audio::au->force_play_cache();
}

void ctrl::set_vol(float vol) {
    audio::au->volume = vol;
    audio::au->update_volume();
}

void ctrl::ch_vol(float delta) {
    audio::au->volume += delta;
    audio::au->update_volume();    
}

void ctrl::set_pos(float pos) {
    audio::au->cur_set_pos(pos);
}

void ctrl::ch_pos(float delta) {
    audio::au->cur_set_pos(audio::au->cur_get_pos() + delta);
}
