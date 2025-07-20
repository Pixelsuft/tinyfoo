#pragma once
#include <lbs.hpp>
#include <rect.hpp>

namespace ren {
    class RendererBase {
        public:
        bool inited;

        RendererBase() {
            inited = false;
        }
        virtual ~RendererBase() {}
        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;
        virtual Point get_size() = 0;
        virtual Point point_win_to_ren(const Point& pos) = 0;
        virtual void* tex_from_io(void* ctx, bool free_src) = 0;
        virtual void tex_destroy(void* tex) = 0;
    };
    extern RendererBase* rn;

    RendererBase* create_renderer_sdl3(void* win);
#if ENABLE_OPENGL3
    RendererBase* create_renderer_opengl3(void* win);
#endif
}
