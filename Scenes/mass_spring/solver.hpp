
#pragma once

#include "mass_spring_system.hpp"

class ISolver
{
public:
    virtual ~ISolver() = default;
    virtual void Init(MassSpringSystem& system, float deltaTime) const noexcept = 0;
    virtual void Step(MassSpringSystem& system, float deltaTime) const noexcept = 0;
};

class EulerSolver final : public ISolver
{
    void Init(MassSpringSystem& system, float deltaTime) const noexcept override;
    void Step(MassSpringSystem& system, float deltaTime) const noexcept override;
};

class LeapFrogSolver final : public ISolver
{
public:
    void Init(MassSpringSystem& system, float deltaTime) const noexcept override;
    void Step(MassSpringSystem& system, float deltaTime) const noexcept override;
};

class MidpointSolver final : public ISolver
{
public:
    void Init(MassSpringSystem& system, float deltaTime) const noexcept override;
    void Step(MassSpringSystem& system, float deltaTime) const noexcept override;
};
