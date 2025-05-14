#include "Application.h"
#include "Cube.h"
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

Application::Application(int width, int height, const char* title)
    : width(width), height(height), title(title), window(nullptr) {
    renderer = new Renderer(width, height, title);  // Create the renderer
    scene = new Scene();  // Create the scene
}

Application::~Application() {
    delete renderer;
    delete scene;
    delete camera;
    delete context;
    context = nullptr;
}

void Application::Start() {
    if (!renderer->Initialize()) return;
    window = renderer->GetWindow();

    context = new WindowContext();
    context->app = this;
    context->renderer = renderer;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(window, context);

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto* ctx = static_cast<WindowContext*>(glfwGetWindowUserPointer(win));
        if (ctx && ctx->app) ctx->app->OnMouseMove(xpos, ypos);
        });

    camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), -90.0f, 0.0f);
    renderer->SetCamera(camera);

    Shader* basicShader = new Shader("basic.vert", "basic.frag", true);

    Piramid* piramid = new Piramid(basicShader);
    piramid->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    piramid->SetColor(glm::vec3(1.0f, 0.5f, 0.2f));
    scene->AddObject(std::unique_ptr<Object3D>(piramid));


    Cube* cube = new Cube(basicShader);
    cube->SetPosition(glm::vec3(-4.0f, 0.0f, 0.0f));
    cube->SetColor(glm::vec3(0.7f, 0.0f, 0.3f));
    scene->AddObject(std::unique_ptr<Object3D>(cube));

    robot.Create(basicShader, scene);
    robot.body->SetPosition(glm::vec3(4.0f, 2.0f, 0.0f));
}

void Application::Run() {
    while (!glfwWindowShouldClose(renderer->GetWindow())) {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        ProcessInput(deltaTime);
        Update(deltaTime);
        scene->Update();

        renderer->Render(scene, renderer->GetViewProj());
    }
}

void Application::OnMouseMove(double xpos, double ypos) {
    if (!rightMousePressed) {
        firstMouse = true;
        return;
    }

    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);  // y is inverted

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera->ProcessMouseMovement(xoffset, yoffset);
}
void Application::ProcessInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    bool forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool up = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    // Detect right mouse button state
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!rightMousePressed) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Lock and hide cursor
            rightMousePressed = true;
        }
    }
    else {
        if (rightMousePressed) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // Show cursor
            rightMousePressed = false;
        }
    }

    camera->ProcessKeyboard(deltaTime, forward, backward, left, right, up, down);
}

void Application::Update(float deltaTime)
{
    robot.Update(deltaTime);
}