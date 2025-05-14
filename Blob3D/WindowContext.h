#pragma once

struct Renderer;
class Application;

struct WindowContext {
    Application* app = nullptr;
    Renderer* renderer = nullptr;
};
