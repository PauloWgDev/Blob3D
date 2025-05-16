#define GLM_ENABLE_EXPERIMENTAL

#include "Application.h"
#include "Cube.h"
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Application::Application(int width, int height, const char* title)
    : width(width), height(height), title(title), window(nullptr) {
    renderer = new Renderer(width, height, title);  // Create the renderer
    scene = new Scene();  // Create the scene
}

Application::~Application() {
    gui.Shutdown();
    delete renderer;
    delete scene;
    delete camera;
    delete context;
    context = nullptr;
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

void Application::OnMouseClick() {
    static bool wasPressedLastFrame = false;

    // Determine GUI panel boundaries (assumes GUI is on the right)
    int winWidth, winHeight;
    glfwGetFramebufferSize(window, &winWidth, &winHeight);
    int guiPanelXStart = winWidth - 300; // assuming 300px wide GUI panel

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // If click is inside GUI region, ignore selection
    if (mouseX >= guiPanelXStart) {
        wasPressedLastFrame = false;
        return;
    }


    bool isPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (isPressed && !wasPressedLastFrame) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        Ray ray = ScreenPosToRay((float)mouseX, (float)mouseY);

        Object3D* hit = nullptr;

        for (const auto& obj : scene->GetObjects()) {
            if (obj->IntersectRay(ray)) {
                hit = obj.get();  // store raw pointer to avoid duplicate search
                break;
            }
        }

        // Only update if it's a different selection
        if (selectedObject != hit) {
            if (selectedObject) selectedObject->SetSelected(false);
            selectedObject = hit;
            if (selectedObject) {
                selectedObject->SetSelected(true);
                std::cout << "Object selected!\n";
                std::cout << "Position: " << glm::to_string(selectedObject->GetPosition()) << "\n";
                std::cout << "Rotation: " << glm::to_string(selectedObject->GetRotationEuler()) << "\n";
                std::cout << "Scale:    " << glm::to_string(selectedObject->GetScale()) << "\n";
            }
        }
    }

    wasPressedLastFrame = isPressed;
}


void Application::HandleObjectSpawning()
{
    ObjectType request = gui.GetSpawnRequest();

    if (request != ObjectType::None) {
        Object3D* newObj = nullptr;

        if (request == ObjectType::Cube) {
            Cube* cube = new Cube(basicShader);  // basicShader must be accessible
            cube->SetPosition(glm::vec3(0, 0, 0));
            cube->SetColor(glm::vec3(0.2f, 0.8f, 0.3f));
            newObj = cube;
        }
        else if (request == ObjectType::Pyramid) {
            Piramid* pyramid = new Piramid(basicShader);
            pyramid->SetPosition(glm::vec3(0, 0, 0));
            pyramid->SetColor(glm::vec3(0.8f, 0.6f, 0.2f));
            newObj = pyramid;
        }

        if (newObj)
            scene->AddObject(std::unique_ptr<Object3D>(newObj));
    }

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


Ray Application::ScreenPosToRay(float mouseX, float mouseY) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    int guiPanelWidth = 300; // same as GUI width in Render
    int viewportWidth = fbWidth - guiPanelWidth;

    float x = (2.0f * (mouseX - 0)) / viewportWidth - 1.0f;  // Adjust if GUI on left
    float y = 1.0f - (2.0f * mouseY) / fbHeight;

    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    glm::mat4 proj = renderer->GetProjection();
    glm::mat4 view = renderer->GetView();

    glm::vec4 rayEye = glm::inverse(proj) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
    glm::vec3 rayOrigin = camera->GetPosition();

    return Ray(rayOrigin, rayWorld);
}

void Application::Run() {
    while (!glfwWindowShouldClose(renderer->GetWindow())) {
        glfwPollEvents();

        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        scene->Update();

        // Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame(); // <-- sets DisplaySize properly
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureKeyboard)
            ProcessInput(deltaTime);

        gui.Render(selectedObject); // GUI rendering must be inside frame

        HandleObjectSpawning();

        ImGui::Render(); // Must come AFTER gui.Render()

        if (!io.WantCaptureMouse)
            OnMouseClick();
        else
            std::cout << "io does want to capture the mouse\n";

        if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            std::cout << "GLFW window is not focused\n";
        }


        renderer->Render(scene, renderer->GetViewProj(), ImGui::GetDrawData());
    }
}

void Application::Start() {
    if (!renderer->Initialize()) return;
    window = renderer->GetWindow();

    gui.Init(window);

    context = new WindowContext();
    context->app = this;
    context->renderer = renderer;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(window, context);

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        ImGui_ImplGlfw_CursorPosCallback(win, xpos, ypos);  // Forward to ImGui
        auto* ctx = static_cast<WindowContext*>(glfwGetWindowUserPointer(win));
        if (ctx && ctx->app) ctx->app->OnMouseMove(xpos, ypos);
        });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        ImGui_ImplGlfw_MouseButtonCallback(win, button, action, mods);  // Forward to ImGui
        });

    glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
        ImGui_ImplGlfw_ScrollCallback(win, xoffset, yoffset);  // Forward to ImGui
        });


    camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), -90.0f, 0.0f);
    renderer->SetCamera(camera);

    basicShader = new Shader("basic.vert", "basic.frag", true);

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

void Application::Update(float deltaTime)
{
    robot.Update(deltaTime);
}