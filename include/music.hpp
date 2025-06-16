#pragma once
#include <str.hpp>

namespace audio {
    struct Music {
        tf::str full_path;
        tf::str fn;
        uint64_t last_click;
        void* h1;
        void* h2;
        float dur;
        bool selected;

        inline Music() {
            h1 = nullptr;
            h2 = nullptr;
            dur = -1.f;
            selected = false;
        }
    };
}
