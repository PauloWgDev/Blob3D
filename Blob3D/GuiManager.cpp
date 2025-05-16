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
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GuiManager::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiManager::Render(Object3D* selected) {
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImVec2 windowSize(300, 180);
    ImVec2 windowPos(displaySize.x - windowSize.x - 10.0f, 10.0f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Selected Object");

    if (selected) {
        auto pos = selected->GetPosition();
        auto rot = selected->GetRotationEuler();
        auto scale = selected->GetScale();

        if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
            selected->SetPosition(pos);

        if (ImGui::DragFloat3("Rotation", &rot.x, 1.0f))
            selected->SetRotationEuler(rot);

        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
            selected->SetScale(scale);
    }
    else {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}


