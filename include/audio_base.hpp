#pragma once

namespace audio {
    struct MusicBase {
        float duration;
    };

    class AudioBase {
        public:
        bool inited;
        bool dev_opened;
        AudioBase();
        virtual ~AudioBase();
        virtual bool dev_open();
        virtual void dev_close();
        virtual MusicBase* mus_from_fp(const char* fp);
        virtual void mus_free(MusicBase* mus);
        virtual bool mus_fill_info(MusicBase* mus);
    };

    extern AudioBase* au;

    AudioBase* create_base();
    AudioBase* create_sdl2_mixer(bool use_mixer_x);
    void free_audio(AudioBase* handle);
}
