#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Object3D;  // Forward declaration

class GuiManager {
public:
    void Init(GLFWwindow* window);
    void Shutdown();
    void Render(Object3D* selected);
};