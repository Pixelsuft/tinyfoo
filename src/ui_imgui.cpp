#include <ui.hpp>
#include <app.hpp>
#include <new.hpp>
#include <log.hpp>
#include <config.hpp>
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
    void draw_playlist_tabs();
    void draw_tab();
    void draw_meta();
    void draw_playlist_view();
}

bool ui::init() {
    ImGuiIO& io = ImGui::GetIO();
    data = tf::bump_nw<UiData>();
    // TODO: customize, handle errors
#if IS_WIN
#define TEMP_FONTS_DIR "c:\\Windows\\Fonts\\"
#else
#define TEMP_FONTS_DIR "/home/lexa/Documents/st/"
#endif
    data->font1 = io.Fonts->AddFontFromFileTTF(TEMP_FONTS_DIR "segoeuib.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
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
    ImGui::Button("|<");
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

void ui::draw_playlist_tabs() {
    if (ImGui::BeginTabBar("PlaylistTabs", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton |
        ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_DrawSelectedOverline)) {
        if (ImGui::BeginTabItem("TestTab", nullptr, 0)) {
            draw_tab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void ui::draw_meta() {
    ImGui::Button("Test1 btn?");
    ImGui::Button("Test2 btn");
    ImGui::Button("Test3 btn");
    ImGui::Button("Test4 btn");
    ImGui::Button("Test 2 btn!!!!!");
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %f", 1.f / io.DeltaTime);
}

void ui::draw_playlist_view() {
    if (ImGui::BeginTable("PlaylistTable", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("File Name");
        ImGui::TableSetupColumn("Duration");
        ImGui::TableSetupColumn("Codec");
        ImGui::TableSetupColumn("Bitrate");
        ImGui::TableSetupColumn("Last Modified");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        static bool test[10000] = { 0 };
        ImGuiListClipper clipper;
        clipper.Begin(10000);
        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 5; column++)
                {
                    char buf[60];
                    SDL_snprintf(buf, 60, "Test %i %i!!!!!!!!!!!!", column, row);
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Selectable(buf, &test[row], ImGuiSelectableFlags_SpanAllColumns);
                }
            }
        }
        ImGui::EndTable();
    }
}

void ui::draw_tab() {
    //ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.f, 0.f });
    if (ImGui::BeginTable("PlaylistTableMain", 2, ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("PlaylistMetaCol", ImGuiTableColumnFlags_IndentDisable);
        ImGui::TableSetupColumn("PlaylistViewCol", ImGuiTableColumnFlags_IndentDisable);
        ImGui::TableNextColumn();
        draw_meta();
        ImGui::TableNextColumn();
        draw_playlist_view();
        ImGui::EndTable();
    }
    //ImGui::PopStyleVar();
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
            draw_menubar();
            ImGui::Separator();
            draw_playback_buttons();
            ImGui::Separator();
            draw_volume_control();
            ImGui::Separator();
            draw_position();
            ImGui::EndMenuBar();
        }
        ImGui::PopFont();
        draw_playlist_tabs();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void ui::destroy() {
    tf::bump_dl(data);
}
