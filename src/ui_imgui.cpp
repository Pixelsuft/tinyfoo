#include <ui.hpp>
#include <app.hpp>
#include <new.hpp>
#include <log.hpp>
#include <imgui.h>
#include <SDL3/SDL.h>

namespace ui {
    struct UiData {
        Point size;
    };

    UiData* data;

    void draw_menubar();
}

bool ui::init() {
    data = tf::bump_nw<UiData>();
    ImGui::StyleColorsDark();
    return true;
}

void ui::update_size(const Point& size) {
    data->size = size;
}

void ui::draw_menubar() {
    // Menu
    // https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp#L641
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", nullptr, nullptr)) {
                app::stop();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Playback")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About", nullptr, nullptr)) {
                TF_INFO(<< "TODO: show about dialog");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void ui::draw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos({ 0.f, 0.f }, 1);
    ImGui::SetNextWindowSize({ data->size.x, data->size.y }, 1);
    // ImGuiWindowFlags_NoBackground ?
    if (ImGui::Begin("tinyfoo", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar)) {
        ui::draw_menubar();
        ImGui::Text("Hello, world!!!");
        ImGui::Button("Test Button");
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void ui::destroy() {
    tf::bump_dl(data);
}
