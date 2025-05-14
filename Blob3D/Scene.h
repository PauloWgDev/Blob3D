#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Shader.h"
#include "Object3D.h"

class Scene {
public:
    Scene();
    ~Scene();

    void AddObject(std::unique_ptr<Object3D> object);  // Singular
    void AddObjects(std::vector<std::unique_ptr<Object3D>> newObjects); // Add multiple Objects at once
    void Update(); 
    void Render(Shader* shader, const glm::mat4& viewProjMatrix);

    const std::vector<std::unique_ptr<Object3D>>& GetObjects() const { return objects; }

private:
    std::vector<std::unique_ptr<Object3D>> objects;  // A list of all 3D objects in the scene
};
