#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include "Shader.h"
#include "Scene.h"
#include "Camera.h"


class Renderer {
public:
    Renderer(int width, int height, const char* title);
    ~Renderer();

    bool Initialize();
    void Render(Scene* scene, const glm::mat4& viewProjMatrix);
    void SetCamera(Camera* cam) { camera = cam; }
    void UpdateProjection(int width, int height);

    GLFWwindow* GetWindow() const { return window; }
    Camera* GetCamera() const { return camera; }
    Shader* GetShader() const { return shader; }

    glm::mat4 GetProjection() const { return projection; }  // Getter for the projection matrix
    glm::mat4 GetView() const {
        return camera ? camera->GetViewMatrix() : glm::mat4(1.0f);
    }
    glm::mat4 GetViewProj() const {
        return GetProjection() * GetView();
    }

private:
    GLFWwindow* window;
    Shader* shader;
    glm::mat4 view;
    glm::mat4 projection;

    Camera* camera = nullptr;

    void SetupCamera();
};
