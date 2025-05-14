#define GLM_ENABLE_EXPERIMENTAL
#include "Object3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

Object3D::Object3D()
    : mesh(nullptr), shader(nullptr), modelMatrix(1.0f){}

Object3D::~Object3D()
{
    if (mesh)
    {
        mesh->cleanup();
        delete mesh;
        mesh = nullptr;
    }
}

void Object3D::Update(float deltaTime)
{
    // Default: do nothing (can be overridden by child classes)
}

void Object3D::Draw(const glm::mat4& viewProjMatrix)
{
    if (shader && mesh)
    {
        shader->use();
        glm::mat4 MVP = viewProjMatrix * modelMatrix;
        shader->setMat4("MVP", glm::value_ptr(MVP));
        shader->setVec3("uColor", glm::value_ptr(color));
        mesh->draw();
    }

    // Recusively draw children
    for (const auto& child : children) {
        if (child) {
            child->Draw(viewProjMatrix);
        }
    }
}

void Object3D::SetPosition(const glm::vec3& pos)
{
    position = pos;
    RebuildLocalTransform();
}

void Object3D::SetRotationEuler(const glm::vec3& rot)
{
    rotationEuler = rot;
    RebuildLocalTransform();
}

void Object3D::SetScale(const glm::vec3& scl)
{
    scale = scl;
    RebuildLocalTransform();
}

void Object3D::SetTransform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) {
    position = pos;
    rotationEuler = rot;
    scale = scl;
    RebuildLocalTransform();
}

void Object3D::SetWorldTransform(const glm::vec3& position, const glm::vec3& rotationEuler, const glm::vec3& scale, const glm::mat4& parentModelMatrix) {
    glm::mat4 worldTransform = glm::mat4(1.0f);
    worldTransform = glm::translate(worldTransform, position);
    worldTransform = glm::rotate(worldTransform, glm::radians(rotationEuler.x), glm::vec3(1, 0, 0));
    worldTransform = glm::rotate(worldTransform, glm::radians(rotationEuler.y), glm::vec3(0, 1, 0));
    worldTransform = glm::rotate(worldTransform, glm::radians(rotationEuler.z), glm::vec3(0, 0, 1));
    worldTransform = glm::scale(worldTransform, scale);

    localTransform = glm::inverse(parentModelMatrix) * worldTransform;
}

void Object3D::SetPivotOffset(const glm::vec3& offset) {
    pivotOffset = offset;
}

void Object3D::AddChild(std::unique_ptr<Object3D> child) {
    children.push_back(std::move(child));
}

void Object3D::ComputeModelMatrix(const glm::mat4& parentTransform) {
    modelMatrix = parentTransform * localTransform;
    for (const auto& child : children) { 
        if (child) child->ComputeModelMatrix(modelMatrix);
    }
}

void Object3D::RebuildLocalTransform()
{
    localTransform = glm::mat4(1.0f);
    localTransform = glm::translate(localTransform, position);
    localTransform = glm::translate(localTransform, -pivotOffset);
    localTransform = glm::rotate(localTransform, glm::radians(rotationEuler.x), glm::vec3(1, 0, 0));
    localTransform = glm::rotate(localTransform, glm::radians(rotationEuler.y), glm::vec3(0, 1, 0));
    localTransform = glm::rotate(localTransform, glm::radians(rotationEuler.z), glm::vec3(0, 0, 1));
    localTransform = glm::scale(localTransform, scale);
    localTransform = glm::translate(localTransform, pivotOffset);
}