
#pragma once

#include <vector>
#include <imgui.h>
#include "../Scene.h"
#include "rigid_body.hpp"
#include "aabb.hpp"

class RigidBodyScene : public Scene
{
private:
    float m_stepTime = 0.01f;
    float m_lag = 0.0f;
    bool m_gravityEnabled = false;

    const int m_simulationSize = 2;
    std::vector<RigidBody> m_rigidBodies;
    AABB m_aabb;

    glm::vec3 m_cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

public:
    virtual void init() override;
    virtual void simulateStep() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;
};