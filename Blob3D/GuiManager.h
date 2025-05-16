#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Object3D;  // Forward declaration

enum class ObjectType {
    None,
    Cube,
    Pyramid
};

class GuiManager {
public:
    void Init(GLFWwindow* window);
    void Shutdown();
    void Render(Object3D* selected);

    ObjectType GetSpawnRequest() {
        ObjectType tmp = spawnRequest;
        spawnRequest = ObjectType::None;
        return tmp;
    }

private:
    ObjectType spawnRequest = ObjectType::None;
};