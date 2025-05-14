#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "WindowContext.h"
#include <glm/gtc/type_ptr.hpp>


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


void Renderer::Render(Scene* scene, const glm::mat4& viewProjMatrix) {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Pass 1: Normal draw with stencil write
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF); // Enable stencil write

    shader->use();
    shader->setMat4("viewProj", &viewProjMatrix[0][0]);
    scene->Render(shader, viewProjMatrix);

    // Pass 2: Draw outlines where stencil != 1
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // Disable stencil write
    glDisable(GL_DEPTH_TEST);

    outlineShader->use();

    for (const auto& obj : scene->GetObjects()) {
        if (obj->IsSelected()) {
            glm::mat4 scaled = obj->GetModelMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(1.05f));
            glm::mat4 mvp = viewProjMatrix * scaled;
            outlineShader->setMat4("MVP", &mvp[0][0]);
            outlineShader->setVec3("uColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
            obj->DrawRawMesh();
        }
    }

    // Restore state
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::UpdateProjection(int width, int height) {
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}