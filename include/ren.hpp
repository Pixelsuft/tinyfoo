#pragma once
#include <lbs.hpp>
#include <rect.hpp>

namespace ren {
    class RendererBase {
        protected:
#if IS_WIN
        long (__stdcall *DwmFlush_ptr)();
#endif
        public:
        bool inited;

        RendererBase();
        virtual ~RendererBase();
        bool init_fake_vsync();
        void do_fake_vsync();
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
    void set_opengl3_attribs();
    RendererBase* create_renderer_opengl3(void* win);
#endif
#if ENABLE_OPENGL2
    void set_opengl2_attribs();
    RendererBase* create_renderer_opengl2(void* win);
#endif
#if ENABLE_DIRECT3D9
    RendererBase* create_renderer_direct3d9(void* win);
#endif
}
