#pragma once
#include <imgui.h>

/*
TODO: cleanup, improvements cuz they are bad
*/

namespace ui
{
    static inline void style_raikiri()
    {
        ImGui::GetStyle().FrameRounding = 4.0f;
        ImGui::GetStyle().GrabRounding = 4.0f;

        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }

    static inline void style_gpulib()
    {
        ImVec4 color_for_text = ImVec4(236.f / 255.f, 240.f / 255.f, 241.f / 255.f, 1.f);
        ImVec4 color_for_head = ImVec4(41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.f);
        ImVec4 color_for_area = ImVec4(57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 1.f);
        ImVec4 color_for_body = ImVec4(44.f / 255.f, 62.f / 255.f, 80.f / 255.f, 1.f);
        ImVec4 color_for_pops = ImVec4(33.f / 255.f, 46.f / 255.f, 60.f / 255.f, 1.f);
        ImGuiStyle &style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.58f);
        style.Colors[ImGuiCol_Border] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.32f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.16f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.39f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);
    }

    static inline void style_dracula()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowRounding = 5.3f;
        style.GrabRounding = style.FrameRounding = 2.3f;
        style.ScrollbarRounding = 5.0f;
        style.FrameBorderSize = 1.0f;
        style.ItemSpacing.y = 6.5f;

        style.Colors[ImGuiCol_Text] = {0.73333335f, 0.73333335f, 0.73333335f, 1.00f};
        style.Colors[ImGuiCol_TextDisabled] = {0.34509805f, 0.34509805f, 0.34509805f, 1.00f};
        style.Colors[ImGuiCol_WindowBg] = {0.23529413f, 0.24705884f, 0.25490198f, 0.94f};
        style.Colors[ImGuiCol_ChildBg] = {0.23529413f, 0.24705884f, 0.25490198f, 0.00f};
        style.Colors[ImGuiCol_PopupBg] = {0.23529413f, 0.24705884f, 0.25490198f, 0.94f};
        style.Colors[ImGuiCol_Border] = {0.33333334f, 0.33333334f, 0.33333334f, 0.50f};
        style.Colors[ImGuiCol_BorderShadow] = {0.15686275f, 0.15686275f, 0.15686275f, 0.00f};
        style.Colors[ImGuiCol_FrameBg] = {0.16862746f, 0.16862746f, 0.16862746f, 0.54f};
        style.Colors[ImGuiCol_FrameBgHovered] = {0.453125f, 0.67578125f, 0.99609375f, 0.67f};
        style.Colors[ImGuiCol_FrameBgActive] = {0.47058827f, 0.47058827f, 0.47058827f, 0.67f};
        style.Colors[ImGuiCol_TitleBg] = {0.04f, 0.04f, 0.04f, 1.00f};
        style.Colors[ImGuiCol_TitleBgCollapsed] = {0.16f, 0.29f, 0.48f, 1.00f};
        style.Colors[ImGuiCol_TitleBgActive] = {0.00f, 0.00f, 0.00f, 0.51f};
        style.Colors[ImGuiCol_MenuBarBg] = {0.27058825f, 0.28627452f, 0.2901961f, 0.80f};
        style.Colors[ImGuiCol_ScrollbarBg] = {0.27058825f, 0.28627452f, 0.2901961f, 0.60f};
        style.Colors[ImGuiCol_ScrollbarGrab] = {0.21960786f, 0.30980393f, 0.41960788f, 0.51f};
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = {0.21960786f, 0.30980393f, 0.41960788f, 1.00f};
        style.Colors[ImGuiCol_ScrollbarGrabActive] = {0.13725491f, 0.19215688f, 0.2627451f, 0.91f};
        // style.Colors[ImGuiCol_ComboBg]               = {0.1f, 0.1f, 0.1f, 0.99f};
        style.Colors[ImGuiCol_CheckMark] = {0.90f, 0.90f, 0.90f, 0.83f};
        style.Colors[ImGuiCol_SliderGrab] = {0.70f, 0.70f, 0.70f, 0.62f};
        style.Colors[ImGuiCol_SliderGrabActive] = {0.30f, 0.30f, 0.30f, 0.84f};
        style.Colors[ImGuiCol_Button] = {0.33333334f, 0.3529412f, 0.36078432f, 0.49f};
        style.Colors[ImGuiCol_ButtonHovered] = {0.21960786f, 0.30980393f, 0.41960788f, 1.00f};
        style.Colors[ImGuiCol_ButtonActive] = {0.13725491f, 0.19215688f, 0.2627451f, 1.00f};
        style.Colors[ImGuiCol_Header] = {0.33333334f, 0.3529412f, 0.36078432f, 0.53f};
        style.Colors[ImGuiCol_HeaderHovered] = {0.453125f, 0.67578125f, 0.99609375f, 0.67f};
        style.Colors[ImGuiCol_HeaderActive] = {0.47058827f, 0.47058827f, 0.47058827f, 0.67f};
        style.Colors[ImGuiCol_Separator] = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
        style.Colors[ImGuiCol_SeparatorHovered] = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
        style.Colors[ImGuiCol_SeparatorActive] = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
        style.Colors[ImGuiCol_ResizeGrip] = {1.00f, 1.00f, 1.00f, 0.85f};
        style.Colors[ImGuiCol_ResizeGripHovered] = {1.00f, 1.00f, 1.00f, 0.60f};
        style.Colors[ImGuiCol_ResizeGripActive] = {1.00f, 1.00f, 1.00f, 0.90f};
        style.Colors[ImGuiCol_PlotLines] = {0.61f, 0.61f, 0.61f, 1.00f};
        style.Colors[ImGuiCol_PlotLinesHovered] = {1.00f, 0.43f, 0.35f, 1.00f};
        style.Colors[ImGuiCol_PlotHistogram] = {0.90f, 0.70f, 0.00f, 1.00f};
        style.Colors[ImGuiCol_PlotHistogramHovered] = {1.00f, 0.60f, 0.00f, 1.00f};
        style.Colors[ImGuiCol_TextSelectedBg] = {0.18431373f, 0.39607847f, 0.79215693f, 0.90f};
    }

    static inline void style_ue4()
    {
        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    }

    static void style_cherry()
    {
#define HI(v) ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v) ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v) ImVec4(0.232f, 0.201f, 0.271f, v)
#define BG(v) ImVec4(0.200f, 0.220f, 0.270f, v)
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

        auto &style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = TEXT(0.78f);
        style.Colors[ImGuiCol_TextDisabled] = TEXT(0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = BG(0.58f);
        style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = BG(1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
        style.Colors[ImGuiCol_TitleBg] = LOW(1.00f);
        style.Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
        style.Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
        style.Colors[ImGuiCol_ButtonActive] = MED(1.00f);
        style.Colors[ImGuiCol_Header] = MED(0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
        style.Colors[ImGuiCol_HeaderActive] = HI(1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = MED(0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = MED(1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
        style.Colors[ImGuiCol_PlotLines] = TEXT(0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = TEXT(0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = BG(0.73f);

        style.WindowPadding = ImVec2(6, 4);
        style.WindowRounding = 0.0f;
        style.FramePadding = ImVec2(5, 2);
        style.FrameRounding = 3.0f;
        style.ItemSpacing = ImVec2(7, 1);
        style.ItemInnerSpacing = ImVec2(1, 1);
        style.TouchExtraPadding = ImVec2(0, 0);
        style.IndentSpacing = 6.0f;
        style.ScrollbarSize = 12.0f;
        style.ScrollbarRounding = 16.0f;
        style.GrabMinSize = 20.0f;
        style.GrabRounding = 2.0f;

        style.WindowTitleAlign.x = 0.50f;

        style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
        style.FrameBorderSize = 0.0f;
        style.WindowBorderSize = 1.0f;
#undef HI
#undef MED
#undef LOW
#undef BG
#undef TEXT
    }

    static inline void style_adobe()
    {
        // AdobeInspired style by nexacopic from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 4.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 4.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 4.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 20.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.TabCloseButtonMinWidthSelected = style.TabCloseButtonMinWidthUnselected = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.1630901098251343f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.08627451211214066f, 0.08627451211214066f, 0.08627451211214066f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.1882352977991104f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1137254908680916f, 0.1137254908680916f, 0.1137254908680916f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8784313797950745f, 0.8784313797950745f, 0.8784313797950745f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.9803921580314636f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1490196138620377f, 0.1490196138620377f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2470588237047195f, 0.2470588237047195f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3294117748737335f, 0.3294117748737335f, 0.3294117748737335f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.3098039329051971f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7490196228027344f, 0.7490196228027344f, 0.7490196228027344f, 0.7803921699523926f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7490196228027344f, 0.7490196228027344f, 0.7490196228027344f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 0.6705882549285889f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.9490196108818054f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.2235294133424759f, 0.2235294133424759f, 0.2235294133424759f, 0.8627451062202454f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.321568638086319f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 0.9725490212440491f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.4235294163227081f, 0.4235294163227081f, 0.4235294163227081f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
    }

    static inline void style_vgui()
    {
        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_Button] = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_Header] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.42f, 0.31f, 0.6f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
        colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.23f, 0.18f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
        colors[ImGuiCol_TabActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        ImGuiStyle &style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        style.WindowRounding = 0.0f;
        style.ChildRounding = 0.0f;
        style.FrameRounding = 0.0f;
        style.PopupRounding = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding = 0.0f;
        style.TabRounding = 0.0f;
    }

    static inline void style_deusex()
    {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        style->FramePadding = ImVec2(4, 2);
        style->ItemSpacing = ImVec2(10, 2);
        style->IndentSpacing = 12;
        style->ScrollbarSize = 10;

        style->WindowRounding = 4;
        style->FrameRounding = 4;
        style->PopupRounding = 4;
        style->ScrollbarRounding = 6;
        style->GrabRounding = 4;
        style->TabRounding = 4;

        style->WindowTitleAlign = ImVec2(1.0f, 0.5f);
        style->WindowMenuButtonPosition = ImGuiDir_Right;

        style->DisplaySafeAreaPadding = ImVec2(4, 4);
    }

    static inline void style_sewer56()
    {
        auto &style = ImGui::GetStyle();
        style.FrameRounding = 4.0f;
        style.WindowBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;
        style.GrabRounding = 4.0f;

        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.71f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.47f, 0.22f, 0.22f, 0.65f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.65f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
        colors[ImGuiCol_Header] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.65f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
        colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
        colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }

    constexpr ImVec4 ColorFromBytes(int r, int g, int b)
    {
        return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
    }

    static inline void style_visualstudio()
    {
        auto &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        const ImVec4 bgColor = ColorFromBytes(37, 37, 38);
        const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
        const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

        const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
        const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
        const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

        const ImVec4 textColor = ColorFromBytes(255, 255, 255);
        const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
        const ImVec4 borderColor = ColorFromBytes(78, 78, 78);

        colors[ImGuiCol_Text] = textColor;
        colors[ImGuiCol_TextDisabled] = textDisabledColor;
        colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
        colors[ImGuiCol_WindowBg] = bgColor;
        colors[ImGuiCol_ChildBg] = bgColor;
        colors[ImGuiCol_PopupBg] = bgColor;
        colors[ImGuiCol_Border] = borderColor;
        colors[ImGuiCol_BorderShadow] = borderColor;
        colors[ImGuiCol_FrameBg] = panelColor;
        colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
        colors[ImGuiCol_FrameBgActive] = panelActiveColor;
        colors[ImGuiCol_TitleBg] = bgColor;
        colors[ImGuiCol_TitleBgActive] = bgColor;
        colors[ImGuiCol_TitleBgCollapsed] = bgColor;
        colors[ImGuiCol_MenuBarBg] = panelColor;
        colors[ImGuiCol_ScrollbarBg] = panelColor;
        colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
        colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
        colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
        colors[ImGuiCol_CheckMark] = panelActiveColor;
        colors[ImGuiCol_SliderGrab] = panelHoverColor;
        colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
        colors[ImGuiCol_Button] = panelColor;
        colors[ImGuiCol_ButtonHovered] = panelHoverColor;
        colors[ImGuiCol_ButtonActive] = panelHoverColor;
        colors[ImGuiCol_Header] = panelColor;
        colors[ImGuiCol_HeaderHovered] = panelHoverColor;
        colors[ImGuiCol_HeaderActive] = panelActiveColor;
        colors[ImGuiCol_Separator] = borderColor;
        colors[ImGuiCol_SeparatorHovered] = borderColor;
        colors[ImGuiCol_SeparatorActive] = borderColor;
        colors[ImGuiCol_ResizeGrip] = bgColor;
        colors[ImGuiCol_ResizeGripHovered] = panelColor;
        colors[ImGuiCol_ResizeGripActive] = lightBgColor;
        colors[ImGuiCol_PlotLines] = panelActiveColor;
        colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
        colors[ImGuiCol_PlotHistogram] = panelActiveColor;
        colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
        colors[ImGuiCol_ModalWindowDimBg] = bgColor;
        colors[ImGuiCol_DragDropTarget] = bgColor;
        colors[ImGuiCol_NavHighlight] = bgColor;
        colors[ImGuiCol_Tab] = bgColor;
        colors[ImGuiCol_TabActive] = panelActiveColor;
        colors[ImGuiCol_TabUnfocused] = bgColor;
        colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
        colors[ImGuiCol_TabHovered] = panelHoverColor;

        style.WindowRounding = 0.0f;
        style.ChildRounding = 0.0f;
        style.FrameRounding = 0.0f;
        style.GrabRounding = 0.0f;
        style.PopupRounding = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.TabRounding = 0.0f;
    }

    static inline void style_imfontstudio() {
        ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_FrameBg]                = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
colors[ImGuiCol_FrameBgActive]          = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
colors[ImGuiCol_CheckMark]              = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
colors[ImGuiCol_SliderGrab]             = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
colors[ImGuiCol_Button]                 = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
colors[ImGuiCol_ButtonHovered]          = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
colors[ImGuiCol_ButtonActive]           = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
colors[ImGuiCol_Header]                 = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
colors[ImGuiCol_HeaderHovered]          = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
colors[ImGuiCol_HeaderActive]           = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
colors[ImGuiCol_Separator]              = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
colors[ImGuiCol_SeparatorActive]        = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
colors[ImGuiCol_ResizeGrip]             = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
colors[ImGuiCol_Tab]                    = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
colors[ImGuiCol_TabHovered]             = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
colors[ImGuiCol_TabActive]              = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
colors[ImGuiCol_TabUnfocused]           = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }

    static inline void style_imfontstudio_red() {
        ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Text]                   = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
colors[ImGuiCol_TextDisabled]           = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
colors[ImGuiCol_FrameBgActive]          = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
colors[ImGuiCol_TitleBgActive]          = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
colors[ImGuiCol_CheckMark]              = ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
colors[ImGuiCol_Button]                 = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
colors[ImGuiCol_ButtonHovered]          = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
colors[ImGuiCol_ButtonActive]           = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
colors[ImGuiCol_Header]                 = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
colors[ImGuiCol_HeaderHovered]          = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
colors[ImGuiCol_HeaderActive]           = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
colors[ImGuiCol_Separator]              = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
colors[ImGuiCol_SeparatorActive]        = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
colors[ImGuiCol_Tab]                    = ImVec4(0.07f, 0.07f, 0.07f, 0.51f);
colors[ImGuiCol_TabHovered]             = ImVec4(0.86f, 0.23f, 0.43f, 0.67f);
colors[ImGuiCol_TabActive]              = ImVec4(0.19f, 0.19f, 0.19f, 0.57f);
colors[ImGuiCol_TabUnfocused]           = ImVec4(0.05f, 0.05f, 0.05f, 0.90f);
colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.13f, 0.13f, 0.13f, 0.74f);
colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }

    static inline void style_janekb04() {
ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
  colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
  colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
  colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
  colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
  colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
  colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
  colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
  colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
  colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
  colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
  colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
  colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
  colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
  colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
  colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
  colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
  colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
  colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
  colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
  colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
  colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding                     = ImVec2(8.00f, 8.00f);
  style.FramePadding                      = ImVec2(5.00f, 2.00f);
  style.CellPadding                       = ImVec2(6.00f, 6.00f);
  style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
  style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
  style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
  style.IndentSpacing                     = 25;
  style.ScrollbarSize                     = 15;
  style.GrabMinSize                       = 10;
  style.WindowBorderSize                  = 1;
  style.ChildBorderSize                   = 1;
  style.PopupBorderSize                   = 1;
  style.FrameBorderSize                   = 1;
  style.TabBorderSize                     = 1;
  style.WindowRounding                    = 7;
  style.ChildRounding                     = 4;
  style.FrameRounding                     = 3;
  style.PopupRounding                     = 4;
  style.ScrollbarRounding                 = 9;
  style.GrabRounding                      = 3;
  style.LogSliderDeadzone                 = 4;
  style.TabRounding                       = 4;
    }
}
