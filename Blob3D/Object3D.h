#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

class Object3D
{
public:
    vec3 position = glm::vec3(0.0f);
    vec3 rotationEuler = glm::vec3(0.0f);
    vec3 scale = glm::vec3(1.0f);

    vec3 color = glm::vec3(1.0f);

    std::vector<Object3D*> children;

    Object3D();
    virtual ~Object3D();

    virtual void Update(float deltaTime);
    virtual void Draw(const mat4& viewProjMatrix);

    void SetPosition(const vec3& position);
    void SetRotationEuler(const vec3& rotation);
    void SetScale(const vec3& scale);


    // Local-space setters
    void SetTransform(const glm::vec3& position, const glm::vec3& rotationEuler, const glm::vec3& scale);
    void SetPivotOffset(const glm::vec3& offset);

    // World-space setter
    void SetWorldTransform(const glm::vec3& position, const glm::vec3& rotationEuler, const glm::vec3& scale, const glm::mat4& parentModelMatrix);

    void SetColor(const vec3& newColor) { color = newColor; }

    void AddChild(Object3D* child);
    
    void ComputeModelMatrix(const glm::mat4& parentTransform); 

    // Apply local TRS with pivot offset, used before parent matrix multiplication
    void RebuildLocalTransform();

protected:
    Mesh* mesh;
    Shader* shader;
    glm::mat4 modelMatrix;
    glm::vec3 pivotOffset = glm::vec3(0.0f);
    glm::mat4 localTransform = glm::mat4(1.0f);
};
