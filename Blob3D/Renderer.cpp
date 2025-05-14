#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "WindowContext.h"

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

    SetupCamera();

    return true;
}

void Renderer::SetupCamera() {
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}


void Renderer::Render(Scene* scene, const glm::mat4& viewProjMatrix) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    // Set the view-projection matrix for the shader
    shader->setMat4("viewProj", &viewProjMatrix[0][0]);

    // Render all objects in the scene
    scene->Render(shader, viewProjMatrix);


    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::UpdateProjection(int width, int height) {
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}