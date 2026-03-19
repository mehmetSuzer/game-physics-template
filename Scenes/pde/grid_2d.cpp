
#include "grid_2d.hpp"

Grid2D::Grid2D(
    int32_t dimX, int32_t dimY, 
    float startX, float startY, 
    float endX, float endY) noexcept : 
    m_dimX(dimX), 
    m_dimY(dimY),
    m_startX(startX),
    m_startY(startY),
    m_endX(endX),
    m_endY(endY),
    m_stepX((endX - startX) / dimX),
    m_stepY((endY - startY) / dimY)
{
    m_values = std::vector<float>(dimX * dimY);
}
    
int32_t Grid2D::GetDimX() const noexcept
{
    return m_dimX;
}

int32_t Grid2D::GetDimY() const noexcept
{
    return m_dimY;
}

float Grid2D::GetStepX() const noexcept
{
    return m_stepX;
}

float Grid2D::GetStepY() const noexcept
{
    return m_stepY;
}

float Grid2D::Get(int32_t x, int32_t y) const noexcept
{
    if (x < 0 || x >= m_dimX || y < 0 || y >= m_dimY) { return 0.0f; }
    const int32_t index = y * m_dimX + x;
    return m_values[index];
}

void Grid2D::Set(int32_t x, int32_t y, float value) noexcept
{
    if (x < 0 || x >= m_dimX || y < 0 || y >= m_dimY) { return; }
    const int32_t index = y * m_dimX + x;
    m_values[index] = value;
}

const std::vector<float>& Grid2D::GetAll() const noexcept
{
    return m_values;
}

void Grid2D::SetAll(const std::vector<float>& values) noexcept
{
    m_values = values;
}

// Sampling with bilinear interpolation on a rectangle
float Grid2D::Sample(float x, float y) const noexcept
{
    // Indices of the nearest point on the grid in the upper left direction
    const int32_t ix = static_cast<int32_t>(std::floor((x - m_startX) / m_stepX));
    const int32_t iy = static_cast<int32_t>(std::floor((y - m_startY) / m_stepY));

    // Real position of the nearest point in the upper left direction
    const float px = m_startX + (ix + 0.5f) * m_stepX;
    const float py = m_startY + (iy + 0.5f) * m_stepY;

    // Interpolation coefficients
    const float alpha = (x - px) / m_stepX;
    const float beta  = (y - py) / m_stepY;

    const float value00 = Get(ix,   iy  );
    const float value01 = Get(ix,   iy+1);
    const float value10 = Get(ix+1, iy  );
    const float value11 = Get(ix+1, iy+1);

    // Interpolate X
    const float interp0 = (1.0f - alpha) * value00 + alpha * value10;
    const float interp1 = (1.0f - alpha) * value01 + alpha * value11;

    // Interpolate Y
    const float interp = (1.0f - beta) * interp0 + beta * interp1;
    return interp;
}

float Grid2D::GetLaplacian(int32_t x, int32_t y) const noexcept
{
    if (x < 0 || x >= m_dimX || y < 0 || y >= m_dimY) { return 0.0f; }
    const float secondDrvX = (Get(x+1, y) - 2.0f * Get(x, y) + Get(x-1, y)) / (m_stepX * m_stepX);
    const float secondDrvY = (Get(x, y+1) - 2.0f * Get(x, y) + Get(x, y-1)) / (m_stepY * m_stepY);
    return secondDrvX + secondDrvY;
}

void Grid2D::Rescale(int32_t dimX, int32_t dimY) noexcept
{
    std::vector<float> values(dimX * dimY);
    int32_t index = 0;

    const float stepX = (m_endX - m_startX) / dimX;
    const float stepY = (m_endY - m_startY) / dimY;

    for (int32_t y = 0; y < dimY; ++y)
    {
        for (int32_t x = 0; x < dimX; ++x)
        {
            // The real position of the point that corresponds to the (x, y) entry in the grid
            const float px = m_startX + (x + 0.5f) * stepX;
            const float py = m_startY + (y + 0.5f) * stepY;

            // Determine the value by sampling from the old grid
            const float value = Sample(px, py);

            // Write to the new grid
            values[index++] = value;
        }
    }

    m_dimX = dimX;
    m_dimY = dimY;

    m_stepX = stepX;
    m_stepY = stepY;

    SetAll(values);
}

void Grid2D::Print() const noexcept
{
    for (int32_t y = 0; y < m_dimY; ++y)
    {
        for (int32_t x = 0; x < m_dimX; ++x)
            std::cout << Get(x, y) << ", ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Grid2D::Draw(Renderer& renderer) const noexcept
{
    const Colormap map("inferno");
    const glm::vec2 screenPosition(0.0f, 0.0f);
    const glm::vec2 screenSize(1.0f, 1.0f);
    renderer.drawImage(m_values, m_dimY, m_dimX, map, screenPosition, screenSize);
}

