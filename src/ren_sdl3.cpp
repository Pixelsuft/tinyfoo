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
#include <imgui_impl_sdlrenderer3.h>
#endif

using ren::RendererBase;

static inline void* create_sdl3_fallback_texture() {
    TF_WARN(<< "Returning fallback texture");
    return nullptr;
}

namespace ren {
    class RendererSDL3 : public RendererBase {
        SDL_Renderer* r;
        public:
        RendererSDL3(void* _win) {
            inited = false;
            SDL_Window* win = (SDL_Window*)_win;
            SDL_PropertiesID props = SDL_CreateProperties();
            SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, win);
            auto ren_str = conf::read_str("renderer", "driver", "");
            if (ren_str.size() == 0)
                display_available_drivers();
            else if (ren_str != "auto")
                SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, ren_str.c_str());
            auto vsync_b = conf::read_bool("renderer", "vsync", true);
            SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, vsync_b ? 1 : 0);
            r = SDL_CreateRendererWithProperties(props);
            if (!r) {
                TF_ERROR(<< "Failed to create SDL renderer (" << SDL_GetError() << ")");
                SDL_ClearProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING);
                // Attempt default renderer
                r = SDL_CreateRendererWithProperties(props);
                if (!r) {
                    TF_FATAL(<< "Failed to create SDL default renderer (" << SDL_GetError() << ")");
                    SDL_DestroyProperties(props);
                    display_available_drivers();
                    return;
                }
            }
            SDL_DestroyProperties(props);
            const char* ren_name = SDL_GetRendererName(r);
            bool fix_vsync = false;
            if (ren_name) {
                tf::str sn(ren_name);
                TF_INFO(<< "Renderer created with " << sn << " driver");
                // FIXME WTF
                if (sn == "direct3d11" || sn == "direct3d12" || sn == "gpu" || sn == "vulkan") {
                    fix_vsync = IS_WIN;
                }
            }
            else
                TF_WARN(<< "Failed to get renderer name (" << SDL_GetError() << ")");
#if ENABLE_IMGUI
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            ImGui_ImplSDL3_InitForSDLRenderer(win, r);
            ImGui_ImplSDLRenderer3_Init(r);
#endif
            if (fix_vsync) {
                if (init_fake_vsync())
                    SDL_SetRenderVSync(r, 0);
            }
            inited = true;
        }

        ~RendererSDL3() {
            if (!inited)
                return;
#if ENABLE_IMGUI
            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
#endif
        }

        void begin_frame() {
            // SDL_SetRenderDrawColorFloat(r, 32.f / 255.f, 32.f / 255.f, 32.f / 255.f, 1.f);
            // SDL_RenderClear(r);
#if ENABLE_IMGUI
            ImVec4 bg_col = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
            SDL_SetRenderDrawColorFloat(r, bg_col.x, bg_col.y, bg_col.z, 1.f);
            SDL_RenderClear(r);
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
#endif
        }

        void end_frame() {
#if ENABLE_IMGUI
            // ImGuiIO& io = ImGui::GetIO();
            ImGui::Render();
            // SDL_SetRenderScale(r, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), r);
#endif
            SDL_RenderPresent(r);
            do_fake_vsync();
        }

        Point get_size() {
            int w_buf, h_buf;
            if (!SDL_GetRenderOutputSize(r, &w_buf, &h_buf)) {
                TF_ERROR(<< "Failed to get renderer output size (" << SDL_GetError() << ")");
                w_buf = 640;
                h_buf = 480;
            }
            Point res;
            res.x = (float)w_buf;
            res.y = (float)h_buf;
            return res;
        }

        Point point_win_to_ren(const Point& pos) {
            Point ret;
            if (!SDL_RenderCoordinatesFromWindow(r, pos.x, pos.y, &ret.x, &ret.y)) {
                TF_WARN(<< "Failed to get renderer coordinates from window (" << SDL_GetError() << ")");
                ret = pos;
            }
            return ret;
        }

        void* tex_from_io(void* ctx, bool free_src) {
            SDL_Surface* surf = (SDL_Surface*)img::surf_from_io(ctx, free_src);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
            if (!tex) {
                TF_ERROR(<< "Failed to create texture from surface (" << SDL_GetError() << ")");
                SDL_DestroySurface(surf);
                return create_sdl3_fallback_texture();
            }
            // Hack for the error image
            if (surf->w == 2 && surf->h == 2 && !SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST))
                TF_WARN(<< "Failed to set texture scale mode (" << SDL_GetError() << ")");
            SDL_DestroySurface(surf);
            return tex;
        }

        void tex_destroy(void* tex) {
            SDL_DestroyTexture((SDL_Texture*)tex);
        }

        void display_available_drivers() {
            int num = SDL_GetNumRenderDrivers();
            if (num <= 0)
                return;
            TF_INFO(<< "Available renderer drivers: ");
            for (int i = 0; i < num; i++)
                TF_INFO(<< tf::nfstr(SDL_GetRenderDriver(i)));
        }
    };
}

RendererBase* ren::create_renderer_sdl3(void* win) {
    return tf::bump_nw<RendererSDL3>(win);
}
