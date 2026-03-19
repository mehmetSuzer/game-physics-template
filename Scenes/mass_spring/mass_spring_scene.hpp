
#pragma once

#include <imgui.h>
#include "../Scene.h"
#include "solver.h"

class MassSpringScene : public Scene
{
private:
    int m_currentMethod = 1; // Euler: 0, Midpoint: 1, LeapFrog: 2
    EulerSolver m_eulerSolver;
    MidpointSolver m_midpointSolver;
    LeapFrogSolver m_leapFrogSolver;
    
    float m_stepTime = 0.005f;
    float m_lag = 0.0f;
    MassSpringSystem m_system;

    glm::vec3 m_cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

public:
    virtual void init() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
};

