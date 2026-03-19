
#pragma once

#include <vector>
#include <cstdint>
#include "Renderer.h"

class Grid2D final
{
private:
    int32_t m_dimX = 0;
    int32_t m_dimY = 0;

    float m_startX = 0.0f;
    float m_startY = 0.0f;

    float m_endX = 0.0f;
    float m_endY = 0.0f;

    float m_stepX = 0.0f;
    float m_stepY = 0.0f;

    std::vector<float> m_values;

public:
    Grid2D() = default;
    
    Grid2D(
        int32_t dimX, int32_t dimY, 
        float startX, float startY, 
        float endX, float endY) noexcept;
    
    int32_t GetDimX() const noexcept;
    int32_t GetDimY() const noexcept;

    float GetStepX() const noexcept;
    float GetStepY() const noexcept;

    float Get(int32_t x, int32_t y) const noexcept;
    void Set(int32_t x, int32_t y, float value) noexcept;

    const std::vector<float>& GetAll() const noexcept;
    void SetAll(const std::vector<float>& values) noexcept;

    float Sample(float x, float y) const noexcept;
    float GetLaplacian(int32_t x, int32_t y) const noexcept;

    void Rescale(int32_t dimX, int32_t dimY) noexcept;

    void Print() const noexcept;
    void Draw(Renderer& renderer) const noexcept;
};

