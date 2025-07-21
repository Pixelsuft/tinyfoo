#include <lbs.hpp>
#if ENABLE_OPENGL2
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
#include <imgui_impl_opengl2.h>
#endif

using ren::RendererBase;

void custom_opengl2_set_attr(SDL_GLAttr attr, int value) {
    if (!SDL_GL_SetAttribute(attr, value))
        TF_WARN(<< "Failed to set SDL OpenGL2 attribute (" << SDL_GetError() << ")");
}

namespace ren {
    class RendererOpenGL2 : public RendererBase {
        SDL_FPoint scale;
        SDL_Window* win;
        SDL_GLContext ctx;
        public:
        RendererOpenGL2(void* _win) {
            win = (SDL_Window*)_win;
            inited = false;
            scale.x = scale.y = 1.f;
            ctx = SDL_GL_CreateContext(win);
            if (!ctx) {
                TF_ERROR(<< "Failed to create OpenGL2 context (" << SDL_GetError() << ")");
                return;
            }
            if (!SDL_GL_MakeCurrent(win, ctx)) {
                TF_ERROR(<< "Failed to set current OpenGL2 context (" << SDL_GetError() << ")");
                SDL_GL_DestroyContext(ctx);
                return;
            }
            auto vsync_b = conf::read_bool("renderer", "vsync", true);
            if (!SDL_GL_SetSwapInterval(vsync_b ? (init_fake_vsync() ? 0 : 1) : 0))
                TF_WARN(<< "Failed to set OpenGL2 swap interval (" << SDL_GetError() << ")");
#if ENABLE_IMGUI
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui_ImplSDL3_InitForOpenGL(win, ctx);
            ImGui_ImplOpenGL2_Init();
#endif
            TF_INFO(<< "OpenGL2 native renderer created");
            inited = true;
        }

        ~RendererOpenGL2() {
            if (!inited)
                return;
#if ENABLE_IMGUI
            ImGui_ImplOpenGL2_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
#endif
            SDL_GL_DestroyContext(ctx);
        }

        void begin_frame() {
#if ENABLE_IMGUI
            ImGuiIO& io = ImGui::GetIO();
            ImVec4 bg_col = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(bg_col.x * bg_col.w, bg_col.y * bg_col.w, bg_col.z * bg_col.w, bg_col.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
#endif
        }

        void end_frame() {
#if ENABLE_IMGUI
            ImGui::Render();
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
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
            glGenTextures(1, &image_texture);
            glBindTexture(GL_TEXTURE_2D, image_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (w == 2 && h == 2) ? GL_NEAREST : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (w == 2 && h == 2) ? GL_NEAREST : GL_LINEAR);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ns->pixels);
            SDL_DestroySurface(ns);
            return (void*)(intptr_t)image_texture;
        }

        void tex_destroy(void* tex) {
            GLuint image_tex = (GLuint)(intptr_t)tex;
            glDeleteTextures(1, &image_tex);
        }

        void* create_fallback_texture() {
            TF_WARN(<< "Returning fallback texture");
            return nullptr;
        }
    };
}

void ren::set_opengl2_attribs() {
    custom_opengl2_set_attr(SDL_GL_DOUBLEBUFFER, 1);
    custom_opengl2_set_attr(SDL_GL_DEPTH_SIZE, 24);
    custom_opengl2_set_attr(SDL_GL_STENCIL_SIZE, 8);
    custom_opengl2_set_attr(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    custom_opengl2_set_attr(SDL_GL_CONTEXT_MINOR_VERSION, 2);
}

RendererBase* ren::create_renderer_opengl2(void* win) {
    return tf::bump_nw<RendererOpenGL2>(win);
}
#endif
