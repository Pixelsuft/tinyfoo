#pragma once
#include <rect.hpp>

namespace ui {
    bool init();
    void draw();
    void destroy();
    void do_extra_stuff();
    void handle_esc();
    void update_size(const Point& size);
}
