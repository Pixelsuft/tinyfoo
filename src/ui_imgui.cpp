#include <ui.hpp>
#include <new.hpp>
#include <log.hpp>
#include <imgui.h>

namespace ui {
    struct UiData {
        int dummy;
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

void ui::draw() {
#if IS_IMGUI
    if (ImGui::Begin("Test window")) {
        ImGui::Text("Hello, world!!!");
        ImGui::Button("Test Button");
    }
    ImGui::End();
#endif
}

void ui::destroy() {
    tf::bump_dl(data);
}
