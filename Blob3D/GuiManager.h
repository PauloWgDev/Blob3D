#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Object3D;  // Forward declaration
class SceneIODelegate;

enum class ObjectType {
    None,
    Cube,
    Pyramid,
    Sphere
};

class GuiManager {
public:
    void Init(GLFWwindow* window, SceneIODelegate* delegate);
    void Shutdown();
    Object3D* Render(Object3D* selected);

    ObjectType GetSpawnRequest() {
        ObjectType tmp = spawnRequest;
        spawnRequest = ObjectType::None;
        return tmp;
    }

private:
    SceneIODelegate* ioDelegate = nullptr;
    ObjectType spawnRequest = ObjectType::None;
};