#include <ren.hpp>
#include <new.hpp>
#include <log.hpp>
#include <lbs.hpp>
#include <conf.hpp>
#include <image.hpp>
#include <SDL3/SDL.h>
#if IS_IMGUI
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#endif

namespace ren {
    struct RenData {
        SDL_Window* win;
        SDL_Renderer* ren;
    };

    RenData* data;
    void display_available_drivers();
}

void ren::display_available_drivers() {
    int num = SDL_GetNumRenderDrivers();
    if (num <= 0)
        return;
    TF_INFO(<< "Available renderer drivers: ");
    for (int i = 0; i < num; i++) {
        TF_INFO(<< SDL_GetRenderDriver(i));
    }
}

bool ren::init(void* win) {
    data = tf::bump_nw<RenData>();
    data->win = (SDL_Window*)win;
    // Should I handle props errors?
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, win);
    if (conf::get().contains("renderer") && conf::get().at("renderer").is_table()) {
        toml::value tab = conf::get().at("renderer");
        auto ren_str = conf::read_str(tab, "driver", "");
        if (ren_str.size() == 0) {
            display_available_drivers();
        }
        else if (ren_str != "auto") {
            SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, ren_str.c_str());
        }
        auto vsync_b = conf::read_bool(tab, "vsync", true);
        SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, vsync_b ? 1 : 0);
    }
    else {
        SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
    }
    data->ren = SDL_CreateRendererWithProperties(props);
    if (!data->ren) {
        TF_FATAL(<< "Failed to create SDL renderer (" << SDL_GetError() << ")");
        SDL_DestroyProperties(props);
        display_available_drivers();
        tf::bump_dl(data);
        return false;
    }
    SDL_DestroyProperties(props);
    const char* ren_name = SDL_GetRendererName(data->ren);
    if (ren_name)
        TF_INFO(<< "Renderer created with " << ren_name << " driver");
    else
        TF_WARN(<< "Failed to get renderer name (" << SDL_GetError() << ")");
#if IS_IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplSDL3_InitForSDLRenderer(data->win, data->ren);
    ImGui_ImplSDLRenderer3_Init(data->ren);
#endif
    return true;
}

void ren::begin_frame() {
    // SDL_SetRenderDrawColorFloat(data->ren, 32.f / 255.f, 32.f / 255.f, 32.f / 255.f, 1.f);
    // SDL_RenderClear(data->ren);
#if IS_IMGUI
    ImVec4 bg_col = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    SDL_SetRenderDrawColorFloat(data->ren, bg_col.x, bg_col.y, bg_col.z, 1.f);
    SDL_RenderClear(data->ren);
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
#endif
}

void ren::end_frame() {
#if IS_IMGUI
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    // SDL_SetRenderScale(data->ren, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), data->ren);
#endif
    SDL_RenderPresent(data->ren);
}

Point ren::get_size() {
    int w_buf, h_buf;
    if (!SDL_GetRenderOutputSize(data->ren, &w_buf, &h_buf)) {
        TF_ERROR(<< "Failed to get renderer output size (" << SDL_GetError() << ")");
        w_buf = 640;
        h_buf = 480;
    }
    Point res;
    res.x = (float)w_buf;
    res.y = (float)h_buf;
    return res;
}

Point ren::point_win_to_ren(const Point& pos) {
    Point ret;
    if (!SDL_RenderCoordinatesFromWindow(data->ren, pos.x, pos.y, &ret.x, &ret.y)) {
        TF_WARN(<< "Failed to get renderer coordinates from window (" << SDL_GetError() << ")");
        ret = pos;
    }
    return ret;
}

void ren::destroy() {
#if IS_IMGUI
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
#endif
    tf::bump_dl(data);
}

static inline void* create_fallback_texture() {
    // TODO
    TF_WARN(<< "Returning fallback texture");
    return nullptr;
}

void* ren::tex_from_io(void* ctx, bool free_src) {
    SDL_Surface* surf = (SDL_Surface*)img::surf_from_io(ctx, free_src);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(data->ren, surf);
    if (!tex) {
        TF_ERROR(<< "Failed to create texture from surface (" << SDL_GetError() << ")");
        SDL_DestroySurface(surf);
        return create_fallback_texture();
    }
    // Hack for error image
    if (surf->w == 2 && surf->h == 2 && !SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST))
        TF_WARN(<< "Failed to set texture scale mode (" << SDL_GetError() << ")");
    SDL_DestroySurface(surf);
    return tex;
}

void ren::tex_destroy(void* tex) {
    SDL_DestroyTexture((SDL_Texture*)tex);
}
