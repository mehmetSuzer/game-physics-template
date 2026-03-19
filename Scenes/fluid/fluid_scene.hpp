
#pragma once

#include <imgui.h>
#include "../Scene.h"
#include "solver.hpp"
#include "obstacle.hpp"

typedef Poly6Kernel Kernel;

class FluidScene : public Scene
{
private:
    static constexpr int32_t SIZE = 15;
    static constexpr float SPACING = 0.1f;

    float m_stepTime = 0.01f;
    float m_lag = 0.0f;
    bool m_paused = true;

    ParticlePool m_pool;
    Container m_container;
    UniformGrid m_grid;
    Solver<Kernel> m_solver;

public:
    virtual void init() override;
    virtual void simulateStep() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;
};

