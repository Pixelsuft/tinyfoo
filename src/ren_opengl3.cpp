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

void custom_opengl3_set_attr(SDL_GLAttr attr, int value) {
    if (!SDL_GL_SetAttribute(attr, value))
        TF_WARN(<< "Failed to set SDL OpenGL attribute (" << SDL_GetError() << ")");
}

void ren::set_opengl3_attribs() {
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    custom_opengl3_set_attr(SDL_GL_CONTEXT_FLAGS, 0);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    const char* glsl_version = "#version 300 es";
    custom_opengl3_set_attr(SDL_GL_CONTEXT_FLAGS, 0);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    custom_opengl3_set_attr(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    const char* glsl_version = "#version 130";
    custom_opengl3_set_attr(SDL_GL_CONTEXT_FLAGS, 0);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
}

RendererBase* ren::create_renderer_opengl3(void* win) {
    return tf::bump_nw<RendererOpenGL3>(win);
}
#endif
