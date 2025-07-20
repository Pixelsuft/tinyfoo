#include <lbs.hpp>
#if ENABLE_OPENGL3
#include <ren.hpp>
#include <new.hpp>
#include <log.hpp>
#include <lbs.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <image.hpp>
#include <SDL3/SDL.h>
#if ENABLE_IMGUI
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#endif

using ren::RendererBase;

namespace ren {
    class RendererOpenGL3 : public RendererBase {
        public:
        RendererOpenGL3(void* _win) {
            inited = false;
            inited = true;
        }

        ~RendererOpenGL3() {
            if (!inited)
                return;
#if ENABLE_IMGUI
#endif
        }

        void begin_frame() {
#if ENABLE_IMGUI
#endif
        }

        void end_frame() {
#if ENABLE_IMGUI
#endif
        }

        Point get_size() {
            Point res;
            res.x = 640;
            res.y = 480;
            return res;
        }

        Point point_win_to_ren(const Point& pos) {
            Point ret;
            return ret;
        }

        void* tex_from_io(void* ctx, bool free_src) {
            return nullptr;
        }

        void tex_destroy(void* tex) {
        }
    };
}

RendererBase* ren::create_renderer_opengl3(void* win) {
    return tf::bump_nw<RendererOpenGL3>(win);
}
#endif
