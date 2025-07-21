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
        PFNGLGENTEXTURESPROC glGenTextures;
        PFNGLBINDTEXTUREPROC glBindTexture;
        PFNGLTEXIMAGE2DPROC glTexImage2D;
        PFNGLTEXPARAMETERIPROC glTexParameteri;
        PFNGLPIXELSTOREIPROC glPixelStorei;
        PFNGLDELETETEXTURESPROC glDeleteTextures;
    };

    class RendererOpenGL3 : public RendererBase {
        OpenGLApi gl;
        SDL_FPoint scale;
        SDL_Window* win;
        SDL_GLContext ctx;
        public:
        RendererOpenGL3(void* _win) {
            // WARN: SDL_GL_LoadLibrary is called in app.cpp before window creation
            win = (SDL_Window*)_win;
            inited = false;
            scale.x = scale.y = 1.f;
            ctx = SDL_GL_CreateContext(win);
            if (!ctx) {
                TF_ERROR(<< "Failed to create OpenGL context (" << SDL_GetError() << ")");
                SDL_GL_UnloadLibrary();
                return;
            }
            OGL3_LOAD_FUNC(PFNGLVIEWPORTPROC, glViewport);
            OGL3_LOAD_FUNC(PFNGLCLEARCOLORPROC, glClearColor);
            OGL3_LOAD_FUNC(PFNGLCLEARPROC, glClear);
            OGL3_LOAD_FUNC(PFNGLGENTEXTURESPROC, glGenTextures);
            OGL3_LOAD_FUNC(PFNGLBINDTEXTUREPROC, glBindTexture);
            OGL3_LOAD_FUNC(PFNGLTEXIMAGE2DPROC, glTexImage2D);
            OGL3_LOAD_FUNC(PFNGLTEXPARAMETERIPROC, glTexParameteri);
            OGL3_LOAD_FUNC(PFNGLPIXELSTOREIPROC, glPixelStorei);
            OGL3_LOAD_FUNC(PFNGLDELETETEXTURESPROC, glDeleteTextures);
            if (!SDL_GL_MakeCurrent(win, ctx)) {
                TF_ERROR(<< "Failed to set current OpenGL context (" << SDL_GetError() << ")");
                SDL_GL_DestroyContext(ctx);
                SDL_GL_UnloadLibrary();
                return;
            }
            // TODO: conf vsync
            if (!SDL_GL_SetSwapInterval(0))
                TF_WARN(<< "Failed to set OpenGL swap interval (" << SDL_GetError() << ")");
            init_fake_vsync();
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
            TF_INFO(<< "OpenGL native renderer created");
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
            do_fake_vsync();
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
            if (SDL_GetWindowSize(win, &w_buf, &h_buf)) {
                scale.x = res.x / (float)w_buf;
                scale.y = res.y / (float)h_buf;
            }
            else
                scale.x = scale.y = 1.f;
            return res;
        }

        Point point_win_to_ren(const Point& pos) {
            Point ret = { pos.x * scale.x, pos.y * scale.y };
            return ret;
        }

        void* tex_from_io(void* ctx, bool free_src) {
            SDL_Surface* surf = (SDL_Surface*)img::surf_from_io(ctx, free_src);
            if (!surf)
                return create_fallback_texture();
            // int w = (int)std::pow(2, (int)std::ceil(std::log2f((float)surf->w)));
            // int h = (int)std::pow(2, (int)std::ceil(std::log2f((float)surf->h)));
            int w = surf->w;
            int h = surf->h;
            SDL_Surface* ns = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ABGR8888);
            if (!ns) {
                TF_ERROR(<< "Failed to create surface copy for texture (" << SDL_GetError() << ")");
                SDL_DestroySurface(surf);
                return create_fallback_texture();
            }
            if (!SDL_BlitSurface(surf, nullptr, ns, nullptr)) {
                TF_ERROR(<< "Failed to blit surface for texture (" << SDL_GetError() << ")");
                SDL_DestroySurface(ns);
                SDL_DestroySurface(surf);
                return create_fallback_texture();
            }
            SDL_DestroySurface(surf);
            GLuint image_texture;
            gl.glGenTextures(1, &image_texture);
            gl.glBindTexture(GL_TEXTURE_2D, image_texture);
            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (w == 2 && h == 2) ? GL_NEAREST : GL_LINEAR);
            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (w == 2 && h == 2) ? GL_NEAREST : GL_LINEAR);
            gl.glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ns->pixels);
            SDL_DestroySurface(ns);
            return (void*)(intptr_t)image_texture;
        }

        void tex_destroy(void* tex) {
            GLuint image_tex = (GLuint)(intptr_t)tex;
            gl.glDeleteTextures(1, &image_tex);
        }

        void* create_fallback_texture() {
            TF_WARN(<< "Returning fallback texture");
            return nullptr;
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
