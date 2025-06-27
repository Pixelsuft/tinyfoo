#pragma once
#include <lbs.hpp>
#include <music.hpp>
#include <vec.hpp>

namespace audio {
    class AudioBase {
        public:
        tf::vec<Music*> cache;
        tf::str need_dev;
        const char* lib_name;
        Music* cur_mus;
        float fade_next_time;
        float fade_stop_time;
        float fade_pause_time;
        float fade_resume_time;
        float volume;
        float max_volume;
        int cache_opened_cnt;
        bool inited;
        bool dev_opened;
        AudioBase();
        virtual ~AudioBase();
        virtual bool dev_open();
        virtual void dev_close();
        virtual void update_volume();
        virtual void update();
        virtual void force_play_cache();
        virtual bool mus_open_fp(Music* mus, const char* fp);
        virtual void mus_close(Music* mus);
        virtual bool mus_opened(Music* mus);
        virtual bool mus_fill_info(Music* mus);
        virtual bool mus_beging_used(Music* mus);
        virtual float cur_get_dur();
        virtual float cur_get_pos();
        virtual void cur_set_pos(float pos);
        virtual void cur_pause();
        virtual void cur_resume();
        virtual bool cur_paused();
        virtual bool cur_stopped();
        virtual void cur_stop();
        void pre_open();
    };

    extern AudioBase* au;

    AudioBase* create_base();
#if ENABLE_SDL2_MIXER
    AudioBase* create_sdl2_mixer(bool use_mixer_x);
#endif
#if ENABLE_FMOD
    AudioBase* create_fmod();
#endif
#if ENABLE_BASS
    AudioBase* create_bass();
#endif
    void free_audio(AudioBase* handle);
}
