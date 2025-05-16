#include "GuiManager.h"
#include "Object3D.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void GuiManager::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);
}

void GuiManager::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiManager::Render(Object3D* selected) {
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImVec2 windowSize(290, 180);
    ImVec2 windowPos(displaySize.x - windowSize.x - 10.0f, 10.0f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);


    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = true; // Show ImGui’s own mouse cursor


    ImGui::Begin("Selected Object");
    ImGui::Text("Mouse capture: %s", ImGui::GetIO().WantCaptureMouse ? "Yes" : "No");
    ImGui::Text("Window hovered: %s", ImGui::IsWindowHovered() ? "Yes" : "No");

    if (selected) {
        auto pos = selected->GetPosition();
        auto rot = selected->GetRotationEuler();
        auto scale = selected->GetScale();
        auto color = selected->GetColor();

        if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
            selected->SetPosition(pos);

        if (ImGui::DragFloat3("Rotation", &rot.x, 1.0f))
            selected->SetRotationEuler(rot);

        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
            selected->SetScale(scale);

        if (ImGui::DragFloat3("Color", &color.x, 0.1f))
            selected->SetColor(color);

    }
    else {
        ImGui::Text("No object selected.");
    }

    ImGui::End();

    // Panel 2: Object Spawner
    ImVec2 spawnPanelPos = ImVec2(windowPos.x, windowPos.y + windowSize.y + 10.0f);
    ImVec2 spawnPanelSize = ImVec2(windowSize.x, 100.0f);
    ImGui::SetNextWindowPos(spawnPanelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(spawnPanelSize, ImGuiCond_Always);
    ImGui::Begin("Spawn Object");

    if (ImGui::Button("Spawn Cube")) {
        spawnRequest = ObjectType::Cube;
    }

    if (ImGui::Button("Spawn Pyramid")) {
        spawnRequest = ObjectType::Pyramid;
    }

    ImGui::End();
}