
#pragma once

#include <cmath>
#include <imgui.h>
#include "../Scene.h"
#include "solver.hpp"
#include "grid_initialiser.hpp"

class PDEScene2 : public Scene
{
private:
    float m_stepTime = 0.01f;
    float m_lag = 0.0f;

    float m_diffusivity = 0.1f;

    int32_t m_dimX = 16;
    int32_t m_dimY = 16;
    int32_t m_dimZ = 16; // 3D temperature map

    int32_t m_method = 0; // 0 -> Explicit, 1 -> Implicit

    float m_simulationSpeed = 1.0f;
    bool m_paused = true;

    Grid3D m_grid;
    ExplicitSolver m_explicitSolver;
    ImplicitSolver m_implicitSolver;
    GridInitialiser m_initialiser;

public:
    virtual void init() override;
    virtual void simulateStep() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;
};

