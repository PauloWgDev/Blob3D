#include "Scene.h"

Scene::Scene() {}

Scene::~Scene() {
    
}

void Scene::AddObject(std::unique_ptr<Object3D> object) {
    objects.push_back(std::move(object));
}

void Scene::AddObjects(std::vector<std::unique_ptr<Object3D>> newObjects) {
    for (auto& obj : newObjects) {
        objects.push_back(std::move(obj));
    }
}

void Scene::Update() {
    for (const std::unique_ptr<Object3D>& obj : objects) {
        obj->ComputeModelMatrix(glm::mat4(1.0f));
    }
}

void Scene::Render(Shader* shader, const glm::mat4& viewProjMatrix)
{
    for (const std::unique_ptr<Object3D>& obj : objects) {
        obj->Draw(viewProjMatrix);
    }
}
