#include "Scene.h"
#include <fstream>
#include <sstream>
#include "Cube.h"
#include "Piramid.h"
#include "Sphere.h"
#include "Robot.h"

Scene::Scene() {}

Scene::~Scene() {}

void Scene::AddObject(std::unique_ptr<Object3D> object) {
    objects.push_back(std::move(object));
}

void Scene::AddObjects(std::vector<std::unique_ptr<Object3D>> newObjects) {
    for (auto& obj : newObjects) {
        objects.push_back(std::move(obj));
    }
}

void Scene::RemoveObject(Object3D* obj) {
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [obj](const std::unique_ptr<Object3D>& o) {
            return o.get() == obj;
        }),
        objects.end());
}

/// <summary>
/// Loops though each object on the scene and updates the its model matrix
/// </summary>
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

void Scene::SaveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    for (const auto& obj : objects) {
        glm::vec3 pos = obj->GetPosition();
        glm::vec3 rot = obj->GetRotationEuler();
        glm::vec3 scale = obj->GetScale();
        glm::vec3 color = obj->GetColor();

        std::string type = "Cube";
        if (dynamic_cast<Piramid*>(obj.get()))
            type = "Pyramid";
        if (dynamic_cast<Sphere*>(obj.get()))
            type = "Sphere";

        out << type << " "
            << pos.x << " " << pos.y << " " << pos.z << " "
            << rot.x << " " << rot.y << " " << rot.z << " "
            << scale.x << " " << scale.y << " " << scale.z << " "
            << color.r << " " << color.g << " " << color.b << "\n";
    }
}

void Scene::LoadFromFile(const std::string& filename, Shader* shader) {
    std::ifstream in(filename);
    if (!in.is_open()) return;

    objects.clear();  // clear current scene

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string type;
        glm::vec3 pos, rot, scale, color;

        ss >> type
            >> pos.x >> pos.y >> pos.z
            >> rot.x >> rot.y >> rot.z
            >> scale.x >> scale.y >> scale.z
            >> color.r >> color.g >> color.b;

        Object3D* obj = nullptr;
        if (type == "Cube")
            obj = new Cube(shader);
        else if (type == "Pyramid")
            obj = new Piramid(shader);
        else if (type == "Sphere")
            obj = new Sphere(shader);

        if (obj) {
            obj->SetTransform(pos, rot, scale);
            obj->SetColor(color);
            objects.push_back(std::unique_ptr<Object3D>(obj));
        }
    }
}