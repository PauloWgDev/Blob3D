#pragma once

#include <glm/glm.hpp>

class Camera {
public:

    Camera(glm::vec3 position, float yaw, float pitch);

    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const { return position; }

    void ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right, bool up, bool down);
    void ProcessMouseMovement(float xoffset, float yoffset);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    void UpdateCameraVectors();
};