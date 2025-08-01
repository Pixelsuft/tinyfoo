#pragma once
#include <rect.hpp>

namespace ui {
    bool init();
    void draw();
    void destroy();
    void do_extra_stuff();
    bool handle_esc();
    void toggle_search();
    void update_size(const Point& size);
}
