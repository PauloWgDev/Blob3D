#pragma once

#include "Cube.h"
#include "Prism.h"
#include "Scene.h"
#include <glm/gtc/constants.hpp>
#include<memory>

using namespace std;

struct Robot {
    Cube* head = nullptr;
    Prism* body = nullptr;
    Cube* right_leg = nullptr;
    Cube* left_leg = nullptr;
    Cube* right_arm = nullptr;
    Cube* left_arm = nullptr;

    void Create(Shader* shader, Scene* scene) {
        // Create torso (root of hierarchy)
        auto b = std::make_unique<Prism>(shader);
        body = b.get();
        body->SetTransform({ 0.0f, 1.0f, 0.0f }, { 0, 0, 0 }, { 1.0f, 1.0f, 1.0f });
        body->SetColor(glm::vec3(1.0f, 0.5f, 1.0f));
        scene->AddObject(std::move(b));

        // Head (child of body)
        auto h = std::make_unique<Cube>(shader);
        head = h.get();
        head->SetTransform({ 0.0f, 1.25f, 0.0f }, { 0, 0, 0 }, { 0.75f, 0.75f, 0.75f });
        head->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
        body->AddChild(move(h));

        // Left arm (child of body)
        auto la = std::make_unique<Cube>(shader);
        left_arm = la.get();
        left_arm->SetPivotOffset(glm::vec3(0.0f, 0.625f, 0.0f));
        left_arm->SetTransform({ -0.75f, 1.5f, 0.0f }, { 0, 0, 120 }, { 0.25f, 1.75f, 0.25f });
        left_arm->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
        body->AddChild(move(la));

        // Right arm (child of body)
        auto ra = std::make_unique<Cube>(shader);
        right_arm = ra.get();
        right_arm->SetPivotOffset(glm::vec3(0.0f, 0.625f, 0.0f));
        right_arm->SetTransform({ 0.75f, 1.5f, 0.0f }, { 0, 0, -120 }, { 0.25f, 1.75f, 0.25f });
        right_arm->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
        body->AddChild(move(ra));

        // Left leg (child of body)
        auto ll = std::make_unique<Cube>(shader);
        left_leg = ll.get();
        left_leg->SetTransform({ -0.3f, -1.0f, 0.0f }, { 0, 0, 0 }, { 0.3f, 2.0f, 0.3f });
        left_leg->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));
        body->AddChild(move(ll));

        // Right leg (child of body)
        auto rl = std::make_unique<Cube>(shader);
        right_leg = rl.get();
        right_leg->SetTransform({ 0.3f, -1.0f, 0.0f }, { 0, 0, 0 }, { 0.3f, 2.0f, 0.3f });
        right_leg->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));
        body->AddChild(move(rl));
    }

    void Update(float deltaTime) {
        if (!body) return;

        static float walkTime = 0.0f;
        walkTime += deltaTime;

        // Calculate swing angle using sine wave
        float legSwing = sin(walkTime * 3.0f) * 30.0f;    // ±30 degrees
        float armSwing = sin(walkTime * 3.0f + glm::pi<float>()) * 30.0f;  // Opposite phase

        // Animate legs
        if (left_leg) {
            left_leg->SetPivotOffset(glm::vec3(0.0f, -1.0f, 0.0f));  // top of leg
            left_leg->SetRotationEuler(glm::vec3(legSwing, 0.0f, 0.0f));
        }
        if (right_leg) {
            right_leg->SetPivotOffset(glm::vec3(0.0f, -1.0f, 0.0f));  // top of leg
            right_leg->SetRotationEuler(glm::vec3(-legSwing, 0.0f, 0.0f));
        }

        // Animate arms
        if (left_arm) {
            left_arm->SetTransform({ -0.75f, 1.5f, 0.0f }, { armSwing, 0.0f, 170.0f }, { 0.25f, 1.75, 0.25f });
        }
        if (right_arm) {
            right_arm->SetTransform({ 0.75f, 1.5f, 0.0f }, { -armSwing, 0.0f, -170.0f }, { 0.25f, 1.75f, 0.25f });
        }

        // Update full hierarchy
        body->ComputeModelMatrix(glm::mat4(1.0f));
    }
};