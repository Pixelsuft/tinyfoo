#include <ui.hpp>
#include <new.hpp>
#include <log.hpp>
#include <imgui.h>
#include <SDL3/SDL.h>

namespace ui {
    struct UiData {
        Point size;
    };

    UiData* data;
}

bool ui::init() {
    data = tf::bump_nw<UiData>();
#if IS_IMGUI
    ImGui::StyleColorsDark();
#endif
    return true;
}

void ui::update_size(const Point& size) {
    data->size = size;
}

void ui::draw() {
#if IS_IMGUI
    ImGui::SetNextWindowPos({ 0.f, 0.f }, 1);
    ImGui::SetNextWindowSize({ data->size.x, data->size.y }, 1);
    if (ImGui::Begin("tinyfoo", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoDecoration)) {
        ImGui::Text("Hello, world!!!");
        ImGui::Button("Test Button");
    }
    ImGui::End();
#endif
}

void ui::destroy() {
    tf::bump_dl(data);
}
