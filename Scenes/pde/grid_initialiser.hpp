
#pragma once

#include <random>
#include "grid_2d.hpp"
#include "grid_3d.hpp"

class GridInitialiser final
{
private:
    std::random_device m_rd;
    std::mt19937 m_rng;

public:
    GridInitialiser() noexcept;

    void Uniform(Grid2D& grid, float low, float high) noexcept;
    void Normal(Grid2D& grid, float mean, float std) noexcept;
    void SmoothX(Grid2D& grid, float low, float high) const noexcept;
    void SmoothY(Grid2D& grid, float low, float high) const noexcept;
 
    void Uniform(Grid3D& grid, float low, float high) noexcept;
    void Normal(Grid3D& grid, float mean, float std) noexcept;
    void SmoothX(Grid3D& grid, float low, float high) const noexcept;
    void SmoothY(Grid3D& grid, float low, float high) const noexcept;
    void SmoothZ(Grid3D& grid, float low, float high) const noexcept;
};

