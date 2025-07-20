#include <lbs.hpp>
#if ENABLE_OPENGL3
#define SDL_OPENGL_1_FUNCTION_TYPEDEFS
#include <ren.hpp>
#include <new.hpp>
#include <log.hpp>
#include <lbs.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <image.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#if ENABLE_IMGUI
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#endif

#define OGL3_LOAD_FUNC(tp, func_name) do { \
    gl.func_name = (tp)SDL_GL_GetProcAddress(#func_name); \
    if (!gl.func_name) { \
        TF_ERROR(<< "Failed to load OpenGL function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_GL_DestroyContext(ctx); \
        SDL_GL_UnloadLibrary(); \
        return; \
    } \
} while (0)

using ren::RendererBase;

namespace ren {
    struct OpenGLApi {
        PFNGLVIEWPORTPROC glViewport;
        PFNGLCLEARCOLORPROC glClearColor;
        PFNGLCLEARPROC glClear;
    };

    class RendererOpenGL3 : public RendererBase {
        OpenGLApi gl;
        SDL_Window* win;
        SDL_GLContext ctx;
        public:
        RendererOpenGL3(void* _win) {
            // WARN: SDL_GL_LoadLibrary is called in app.cpp before window creation
            win = (SDL_Window*)_win;
            inited = false;
            ctx = SDL_GL_CreateContext(win);
            if (!ctx) {
                TF_ERROR(<< "Failed to create OpenGL context (" << SDL_GetError() << ")");
                SDL_GL_UnloadLibrary();
                return;
            }
            OGL3_LOAD_FUNC(PFNGLVIEWPORTPROC, glViewport);
            OGL3_LOAD_FUNC(PFNGLCLEARCOLORPROC, glClearColor);
            OGL3_LOAD_FUNC(PFNGLCLEARPROC, glClear);
            if (!SDL_GL_MakeCurrent(win, ctx)) {
                TF_ERROR(<< "Failed to set current OpenGL context (" << SDL_GetError() << ")");
                SDL_GL_DestroyContext(ctx);
                SDL_GL_UnloadLibrary();
                return;
            }
            // TODO: conf vsync
            // TODO: it doesn't work
            if (!SDL_GL_SetSwapInterval(1))
                TF_WARN(<< "Failed to set OpenGL swap interval (" << SDL_GetError() << ")");
#if ENABLE_IMGUI
#if defined(IMGUI_IMPL_OPENGL_ES2)
            const char* glsl_version = "#version 100";
#elif defined(IMGUI_IMPL_OPENGL_ES3)
            const char* glsl_version = "#version 300 es";
#elif defined(__APPLE__)
            const char* glsl_version = "#version 150";
#else
            const char* glsl_version = "#version 130";
#endif
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui_ImplSDL3_InitForOpenGL(win, ctx);
            ImGui_ImplOpenGL3_Init(glsl_version);
#endif
            inited = true;
        }

        ~RendererOpenGL3() {
            if (!inited)
                return;
#if ENABLE_IMGUI
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
#endif
            SDL_GL_DestroyContext(ctx);
            SDL_GL_UnloadLibrary();
        }

        void begin_frame() {
#if ENABLE_IMGUI
            ImGuiIO& io = ImGui::GetIO();
            ImVec4 bg_col = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
            gl.glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            gl.glClearColor(bg_col.x * bg_col.w, bg_col.y * bg_col.w, bg_col.z * bg_col.w, bg_col.w);
            gl.glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
#endif
        }

        void end_frame() {
#if ENABLE_IMGUI
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(win);
#endif
        }

        Point get_size() {
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
            Point ret = pos;
            // TODO: high DPI
            return ret;
        }

        void* tex_from_io(void* ctx, bool free_src) {
            // TODO
            return nullptr;
        }

        void tex_destroy(void* tex) {
            // TODO
        }
    };
}

void custom_opengl3_set_attr(SDL_GLAttr attr, int value) {
    if (!SDL_GL_SetAttribute(attr, value))
        TF_WARN(<< "Failed to set SDL OpenGL attribute (" << SDL_GetError() << ")");
}

void ren::set_opengl3_attribs() {
#if defined(IMGUI_IMPL_OPENGL_ES2)
    custom_opengl3_set_attr(SDL_GL_CONTEXT_FLAGS, 0);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    custom_opengl3_set_attr(SDL_GL_CONTEXT_FLAGS, 0);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    custom_opengl3_set_attr(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    custom_opengl3_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
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
