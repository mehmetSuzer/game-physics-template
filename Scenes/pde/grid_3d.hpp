
#pragma once

#include <vector>
#include <cstdint>
#include "Renderer.h"

class Grid3D final
{
private:
    int32_t m_dimX = 0;
    int32_t m_dimY = 0;
    int32_t m_dimZ = 0;

    float m_startX = 0.0f;
    float m_startY = 0.0f;
    float m_startZ = 0.0f;

    float m_endX = 0.0f;
    float m_endY = 0.0f;
    float m_endZ = 0.0f;

    float m_stepX = 0.0f;
    float m_stepY = 0.0f;
    float m_stepZ = 0.0f;

    std::vector<float> m_values;

public:
    Grid3D() = default;

    Grid3D(
        int32_t dimX, int32_t dimY, int32_t dimZ, 
        float startX, float startY, float startZ,
        float endX, float endY, float endZ) noexcept;
    
    int32_t GetDimX() const noexcept;
    int32_t GetDimY() const noexcept;
    int32_t GetDimZ() const noexcept;

    float GetStepX() const noexcept;
    float GetStepY() const noexcept;
    float GetStepZ() const noexcept;

    float Get(int32_t x, int32_t y, int32_t z) const noexcept;
    void Set(int32_t x, int32_t y, int32_t z, float value) noexcept;

    const std::vector<float>& GetAll() const noexcept;
    void SetAll(const std::vector<float>& values) noexcept;

    float Sample(float x, float y, float z) const noexcept;
    float GetLaplacian(int32_t x, int32_t y, int32_t z) const noexcept;

    void Rescale(int32_t dimX, int32_t dimY, int32_t dimZ) noexcept;

    void Print() const noexcept;
    void Draw(Renderer& renderer) const noexcept;
};

