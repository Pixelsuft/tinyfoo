#include <lbs.hpp>
#if ENABLE_DIRECT3D9
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
#include <imgui_impl_dx9.h>
#endif

using ren::RendererBase;

namespace ren {
    class RendererDirect3D9 : public RendererBase {
        SDL_Window* win;
        public:
        RendererDirect3D9(void* _win) {
            // TODO
            win = (SDL_Window*)_win;
            inited = false;
            inited = true;
        }

        ~RendererDirect3D9() {
            // TODO
            if (!inited)
                return;
#if ENABLE_IMGUI
#endif
        }

        void begin_frame() {
            // TODO
#if ENABLE_IMGUI
#endif
        }

        void end_frame() {
            // TODO
#if ENABLE_IMGUI
#endif
        }

        Point get_size() {
            // TODO
            int w_buf, h_buf;
            if (!SDL_GetWindowSizeInPixels(win, &w_buf, &h_buf)) {
                TF_ERROR(<< "Failed to get window size in pixels (" << SDL_GetError() << ")");
                w_buf = 640;
                h_buf = 480;
            }
            Point res;
            res.x = (float)w_buf;
            res.y = (float)h_buf;
            return res;
        }

        Point point_win_to_ren(const Point& pos) {
            // TODO
            Point ret = { pos.x, pos.y };
            return ret;
        }

        void* tex_from_io(void* ctx, bool free_src) {
            // TODO
            return nullptr;
        }

        void tex_destroy(void* tex) {
            // TODO
        }

        void* create_fallback_texture() {
            TF_WARN(<< "Returning fallback texture");
            return nullptr;
        }
    };
}

RendererBase* ren::create_renderer_direct3d9(void* win) {
    return tf::bump_nw<RendererDirect3D9>(win);
}
#endif
