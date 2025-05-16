#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "WindowContext.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Renderer::Renderer(int width, int height, const char* title)
    : window(nullptr), shader(nullptr), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)) {}

Renderer::~Renderer() {
    if (window) {
        glfwSetFramebufferSizeCallback(window, nullptr);
        glfwSetWindowUserPointer(window, nullptr);        
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

bool Renderer::Initialize() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    window = glfwCreateWindow(800, 600, "Blob3D", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        auto* ctx = static_cast<WindowContext*>(glfwGetWindowUserPointer(win));
        if (ctx && ctx->renderer) {
            ctx->renderer->UpdateProjection(width, height);
            glViewport(0, 0, width, height);
        }
        });

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW\n";
        return false;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    shader = new Shader("basic.vert", "basic.frag", false);
    outlineShader = new Shader("basic.vert", "outline.frag", false);

    SetupCamera();

    return true;
}

void Renderer::SetupCamera() {
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}


void Renderer::Render(Scene* scene, const glm::mat4& viewProjMatrix, ImDrawData* imguiDrawData) {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    int guiPanelWidth = 300;
    int viewportWidth = windowWidth - guiPanelWidth;

    // Set viewport only to 3D area (left side)
    glViewport(0, 0, viewportWidth, windowHeight);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // Dark background for 3D area
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Normal 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    shader->use();
    shader->setMat4("viewProj", &viewProjMatrix[0][0]);
    scene->Render(shader, viewProjMatrix);

    // Outline pass
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    outlineShader->use();

    for (const auto& obj : scene->GetObjects()) {
        if (obj->IsSelected()) {
            glm::mat4 scaled = obj->GetModelMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(1.05f));
            glm::mat4 mvp = viewProjMatrix * scaled;
            outlineShader->setMat4("MVP", &mvp[0][0]);
            outlineShader->setVec3("uColor", glm::value_ptr(glm::vec3(1.0f)));
            obj->DrawRawMesh();
        }
    }

    // Reset
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    // Now restore full window for GUI (draws on top, no need to clear)
    glViewport(0, 0, windowWidth, windowHeight);
    if (imguiDrawData)
        ImGui_ImplOpenGL3_RenderDrawData(imguiDrawData);

    glfwSwapBuffers(window);
    glfwPollEvents();
}



void Renderer::UpdateProjection(int windowWidth, int windowHeight) {
    int guiPanelWidth = 300;
    int viewportWidth = windowWidth - guiPanelWidth;

    // Ensure non-zero and valid aspect ratio
    if (viewportWidth <= 0 || windowHeight <= 0)
        return;

    float aspect = static_cast<float>(viewportWidth) / static_cast<float>(windowHeight);
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

