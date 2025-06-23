#pragma once
#include <rect.hpp>

namespace ren {
    bool init(void* win);
    void begin_frame();
    void end_frame();
    void destroy();
    Point get_size();
    Point point_win_to_ren(const Point& pos);
    void* tex_from_io(void* ctx, bool free_src);
    void tex_destroy(void* tex);
}
