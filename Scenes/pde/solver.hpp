
#pragma once

#include "grid_2d.hpp"
#include "grid_3d.hpp"

// Interface for solvers
class ISolver
{
public:
    virtual ~ISolver() = default;

    virtual void Step(Grid2D& grid, float stepTime, float diffusivity) const noexcept = 0;
    virtual void Step(Grid3D& grid, float stepTime, float diffusivity) const noexcept = 0;
};

class ExplicitSolver final : public ISolver
{
public:
    void Step(Grid2D& grid, float stepTime, float diffusivity) const noexcept override;
    void Step(Grid3D& grid, float stepTime, float diffusivity) const noexcept override;
};

class ImplicitSolver final : public ISolver
{
public:
    void Step(Grid2D& grid, float stepTime, float diffusivity) const noexcept override;
    void Step(Grid3D& grid, float stepTime, float diffusivity) const noexcept override;
};

