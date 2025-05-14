#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Cube.h"
#include <GLFW/glfw3.h>
#include "WindowContext.h"
#include "Robot.h"
#include "Piramid.h"

class Application {
public:
    Application(int width, int height, const char* title);
    ~Application();

    void Start(); // Initialization
    void Run();   // Main render loop

private:
    int width, height;
    const char* title;

    WindowContext* context = nullptr;
    Renderer* renderer;  
    Scene* scene;
    Camera* camera;

    GLFWwindow* window;

    float lastFrameTime = glfwGetTime(); // used for DeltaTime

    //bool Initialize();  // Initializes everything
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void OnMouseMove(double xpos, double ypos);

    Robot robot;

    bool rightMousePressed = false;
    float lastX = 400.0f;  // center of window
    float lastY = 300.0f;
    bool firstMouse = true;
};