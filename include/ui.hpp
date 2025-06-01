#pragma once
#include <rect.hpp>

namespace ui {
    bool init();
    void draw();
    void destroy();
    void update_size(const Point& size);
}
