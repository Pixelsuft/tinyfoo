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
        SDL_Renderer* ren;
    };

    RenData* data;
}

bool ren::init(void* win) {
    data = tf::bump_nw<RenData>();
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
    ImGui_ImplSDL3_InitForSDLRenderer((SDL_Window*)win, data->ren);
    ImGui_ImplSDLRenderer3_Init(data->ren);
#endif
    return true;
}

void ren::destroy() {
#if IS_IMGUI
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
#endif
    tf::bump_dl(data);
}
