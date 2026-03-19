
#include "grid_initialiser.hpp"

GridInitialiser::GridInitialiser() noexcept :
    m_rng(m_rd())
{

}

void GridInitialiser::Uniform(Grid2D& grid, float low, float high) noexcept
{
    std::uniform_real_distribution<float> dist(low, high);

    for (int32_t y = 0; y < grid.GetDimY(); ++y)
    {
        for (int32_t x = 0; x < grid.GetDimX(); ++x)
        {
            const float value = dist(m_rng);
            grid.Set(x, y, value);
        }
    }
}

void GridInitialiser::Normal(Grid2D& grid, float mean, float std) noexcept
{
    std::normal_distribution<float> dist(mean, std);

    for (int32_t y = 0; y < grid.GetDimY(); ++y)
    {
        for (int32_t x = 0; x < grid.GetDimX(); ++x)
        {
            const float value = dist(m_rng);
            grid.Set(x, y, value);
        }
    }
}

void GridInitialiser::SmoothX(Grid2D& grid, float low, float high) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();

    for (int32_t y = 0; y < dimY; ++y)
    {
        for (int32_t x = 0; x < dimX; ++x)
        {
            const float value = low + x * (high - low) / (dimX-1);
            grid.Set(x, y, value);
        }
    }
}

void GridInitialiser::SmoothY(Grid2D& grid, float low, float high) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();

    for (int32_t y = 0; y < dimY; ++y)
    {
        for (int32_t x = 0; x < dimX; ++x)
        {
            const float value = low + y * (high - low) / (dimY-1);
            grid.Set(x, y, value);
        }
    }
}

void GridInitialiser::Uniform(Grid3D& grid, float low, float high) noexcept
{
    std::uniform_real_distribution<float> dist(low, high);

    for (int32_t z = 0; z < grid.GetDimZ(); ++z)
    {
        for (int32_t y = 0; y < grid.GetDimY(); ++y)
        {
            for (int32_t x = 0; x < grid.GetDimX(); ++x)
            {
                const float value = dist(m_rng);
                grid.Set(x, y, z, value);
            }
        }
    }
}

void GridInitialiser::Normal(Grid3D& grid, float mean, float std) noexcept
{
    std::normal_distribution<float> dist(mean, std);

    for (int32_t z = 0; z < grid.GetDimZ(); ++z)
    {
        for (int32_t y = 0; y < grid.GetDimY(); ++y)
        {
            for (int32_t x = 0; x < grid.GetDimX(); ++x)
            {
                const float value = dist(m_rng);
                grid.Set(x, y, z, value);
            }
        }
    }
}

void GridInitialiser::SmoothX(Grid3D& grid, float low, float high) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();
    const int32_t dimZ = grid.GetDimZ();

    for (int32_t z = 0; z < dimZ; ++z)
    {
        for (int32_t y = 0; y < dimY; ++y)
        {
            for (int32_t x = 0; x < dimX; ++x)
            {
                const float value = low + x * (high - low) / (dimX-1);
                grid.Set(x, y, z, value);
            }
        }
    }
}

void GridInitialiser::SmoothY(Grid3D& grid, float low, float high) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();
    const int32_t dimZ = grid.GetDimZ();

    for (int32_t z = 0; z < dimZ; ++z)
    {
        for (int32_t y = 0; y < dimY; ++y)
        {
            for (int32_t x = 0; x < dimX; ++x)
            {
                const float value = low + y * (high - low) / (dimY-1);
                grid.Set(x, y, z, value);
            }
        }
    }
}

void GridInitialiser::SmoothZ(Grid3D& grid, float low, float high) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();
    const int32_t dimZ = grid.GetDimZ();
    
    for (int32_t z = 0; z < dimZ; ++z)
    {
        for (int32_t y = 0; y < dimY; ++y)
        {
            for (int32_t x = 0; x < dimX; ++x)
            {
                const float value = low + z * (high - low) / (dimZ-1);
                grid.Set(x, y, z, value);
            }
        }
    }
}

