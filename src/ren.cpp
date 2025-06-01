#include <ren.hpp>
#include <new.hpp>
#include <log.hpp>
#include <config.hpp>
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
}

bool ren::init(void* win) {
    data = tf::bump_nw<RenData>();
    data->win = (SDL_Window*)win;
    // Should I handle props errors?
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, win);
    SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, "direct3d"); // TODO
    SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
    data->ren = SDL_CreateRendererWithProperties(props);
    if (!data->ren) {
        TF_FATAL(<< "Failed to create SDL renderer (" << SDL_GetError() << ")");
        SDL_DestroyProperties(props);
        tf::bump_dl(data);
        return false;
    }
    SDL_DestroyProperties(props);
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
    SDL_SetRenderDrawColorFloat(data->ren, 32.f / 255.f, 32.f / 255.f, 32.f / 255.f, 1.f);
    SDL_RenderClear(data->ren);
#if IS_IMGUI
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

void ren::destroy() {
#if IS_IMGUI
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
#endif
    tf::bump_dl(data);
}
