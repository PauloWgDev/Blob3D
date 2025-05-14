#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/normalize_dot.hpp>
#include <cmath>

Camera::Camera(glm::vec3 position, float yaw, float pitch)
    : position(position),
    yaw(yaw),
    pitch(pitch),
    speed(10.0f),
    sensitivity(0.1f),
    worldUp(glm::vec3(0.0f, 1.0f, 0.0f)) {

    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right, bool up, bool down) {
    float velocity = speed * deltaTime;
    if (forward)
        position += front * velocity;
    if (backward)
        position -= front * velocity;
    if (left)
        position -= this->right * velocity;
    if (right)
        position += this->right * velocity;
    if (up)
        position += this->up * velocity;
    if (down)
        position -= this->up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Recalculate Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors
    up = glm::normalize(glm::cross(right, front));
}
