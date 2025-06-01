#include <ui.hpp>
#include <app.hpp>
#include <new.hpp>
#include <log.hpp>
#include <imgui.h>
#include <SDL3/SDL.h>

namespace ui {
    struct UiData {
        ImFont* font1;
        Point size;
    };

    UiData* data;

    void draw_menubar();
    void draw_playback_buttons();
    void draw_volume_control();
    void draw_position();
}

bool ui::init() {
    ImGuiIO& io = ImGui::GetIO();
    data = tf::bump_nw<UiData>();
    // TODO: customize, handle errors
    data->font1 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeuib.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    if (!data->font1) {
        TF_ERROR(<< "Failed to load ImGui Font 1");
    }
    ImGui::StyleColorsDark();
    return true;
}

void ui::update_size(const Point& size) {
    data->size = size;
}

void ui::draw_menubar() {
    // Menu
    // https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp#L641
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
}

void ui::draw_playback_buttons() {
    ImGui::Button("0");
    ImGui::Button(">");
    ImGui::Button("||");
    ImGui::Button("<|");
    ImGui::Button(">|");
    ImGui::Button(">?");
}

void ui::draw_volume_control() {
    static float vol = 1.f;
    ImGui::PushID("VolumeSlider");
    ImGui::PushItemWidth(100.f);
    ImGui::SliderFloat("", &vol, 0.f, 1.f, "", ImGuiSliderFlags_NoRoundToFormat);
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void ui::draw_position() {
    static float pos = 0.f;
    ImVec2 hacky_rect = ImGui::GetItemRectMax();
    ImGui::PushID("PositionSlider");
    ImGui::PushItemWidth(data->size.x - (hacky_rect.x + 16.f));
    ImGui::SliderFloat("", &pos, 0.f, 1.f, "", ImGuiSliderFlags_NoRoundToFormat);
    ImGui::PopItemWidth();
    ImGui::PopID();
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
        ImGui::PushFont(data->font1);
        if (ImGui::BeginMenuBar()) {
            ui::draw_menubar();
            ImGui::Separator();
            ui::draw_playback_buttons();
            ImGui::Separator();
            ui::draw_volume_control();
            ImGui::Separator();
            ui::draw_position();
            ImGui::EndMenuBar();
        }
        ImGui::PopFont();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void ui::destroy() {
    tf::bump_dl(data);
}
