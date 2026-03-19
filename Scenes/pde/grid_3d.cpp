
#include "grid_3d.hpp"

Grid3D::Grid3D(
    int32_t dimX, int32_t dimY, int32_t dimZ,
    float startX, float startY, float startZ,
    float endX, float endY, float endZ) noexcept : 
    m_dimX(dimX), 
    m_dimY(dimY),
    m_dimZ(dimZ),
    m_startX(startX),
    m_startY(startY),
    m_startZ(startZ),
    m_endX(endX),
    m_endY(endY),
    m_endZ(endZ),
    m_stepX((endX - startX) / dimX),
    m_stepY((endY - startY) / dimY),
    m_stepZ((endZ - startZ) / dimZ)
{
    m_values = std::vector<float>(dimX * dimY * dimZ);
}
    
int32_t Grid3D::GetDimX() const noexcept
{
    return m_dimX;
}

int32_t Grid3D::GetDimY() const noexcept
{
    return m_dimY;
}

int32_t Grid3D::GetDimZ() const noexcept
{
    return m_dimZ;
}

float Grid3D::GetStepX() const noexcept
{
    return m_stepX;
}

float Grid3D::GetStepY() const noexcept
{
    return m_stepY;
}

float Grid3D::GetStepZ() const noexcept
{
    return m_stepZ;
}

float Grid3D::Get(int32_t x, int32_t y, int32_t z) const noexcept
{
    if (x < 0 || x >= m_dimX || y < 0 || y >= m_dimY || z < 0 || z >= m_dimZ) { return 0.0f; }
    const int32_t index = (z * m_dimY + y) * m_dimX + x;
    return m_values[index];
}

void Grid3D::Set(int32_t x, int32_t y, int32_t z, float value) noexcept
{
    if (x < 0 || x >= m_dimX || y < 0 || y >= m_dimY || z < 0 || z >= m_dimZ) { return; }
    const int32_t index = (z * m_dimY + y) * m_dimX + x;
    m_values[index] = value;
}

const std::vector<float>& Grid3D::GetAll() const noexcept
{
    return m_values;
}

void Grid3D::SetAll(const std::vector<float>& values) noexcept
{
    m_values = values;
}

// Trilinear interpolation on a cuboid
float Grid3D::Sample(float x, float y, float z) const noexcept
{
    // Indices of the nearest point on the grid in the upper left forward direction
    const int32_t ix = static_cast<int32_t>(std::floor((x - m_startX) / m_stepX));
    const int32_t iy = static_cast<int32_t>(std::floor((y - m_startY) / m_stepY));
    const int32_t iz = static_cast<int32_t>(std::floor((z - m_startZ) / m_stepZ));

    // Real position of the nearest point in the upper left forward direction
    const float px = m_startX + (ix + 0.5f) * m_stepX;
    const float py = m_startY + (iy + 0.5f) * m_stepY;
    const float pz = m_startZ + (iz + 0.5f) * m_stepZ;

    // Interpolation coefficients
    const float alpha = (x - px) / m_stepX;
    const float beta  = (y - py) / m_stepY;
    const float gamma = (z - pz) / m_stepZ;

    const float value000 = Get(ix,   iy  , z  );
    const float value001 = Get(ix,   iy  , z+1);
    const float value010 = Get(ix,   iy+1, z  );
    const float value011 = Get(ix,   iy+1, z+1);
    const float value100 = Get(ix+1, iy  , z  );
    const float value101 = Get(ix+1, iy  , z+1);
    const float value110 = Get(ix+1, iy+1, z  );
    const float value111 = Get(ix+1, iy+1, z+1);

   // Interpolate X
    const float interp00 = (1.0f - alpha) * value000 + alpha * value100;
    const float interp10 = (1.0f - alpha) * value010 + alpha * value110;
    const float interp01 = (1.0f - alpha) * value001 + alpha * value101;
    const float interp11 = (1.0f - alpha) * value011 + alpha * value111;

    // Interpolate Y
    const float interp0 = (1.0f - beta) * interp00 + beta * interp10;
    const float interp1 = (1.0f - beta) * interp01 + beta * interp11;
 
    // Interpolate Z
    const float interp = (1.0f - gamma) * interp0 + gamma * interp1;
    return interp;
}


float Grid3D::GetLaplacian(int32_t x, int32_t y, int32_t z) const noexcept
{
    if (x < 0 || x >= m_dimX || y < 0 || y >= m_dimY || z < 0 || z >= m_dimZ) { return 0.0f; }
    const float secondDrvX = (Get(x+1, y, z) - 2.0f * Get(x, y, z) + Get(x-1, y, z)) / (m_stepX * m_stepX);
    const float secondDrvY = (Get(x, y+1, z) - 2.0f * Get(x, y, z) + Get(x, y-1, z)) / (m_stepY * m_stepY);
    const float secondDrvZ = (Get(x, y, z+1) - 2.0f * Get(x, y, z) + Get(x, y, z-1)) / (m_stepZ * m_stepZ);
    return secondDrvX + secondDrvY + secondDrvZ;
}

void Grid3D::Rescale(int32_t dimX, int32_t dimY, int32_t dimZ) noexcept
{
    std::vector<float> values(dimX * dimY * dimZ);
    int32_t index = 0;

    const float stepX = (m_endX - m_startX) / dimX;
    const float stepY = (m_endY - m_startY) / dimY;
    const float stepZ = (m_endZ - m_startZ) / dimZ;

    for (int32_t z = 0; z < dimZ; ++z)
    for (int32_t y = 0; y < dimY; ++y)
    for (int32_t x = 0; x < dimX; ++x)
    {
        // The real position of the point that corresponds to the (x, y, z) entry in the grid
        const float px = m_startX + (x + 0.5f) * stepX;
        const float py = m_startY + (y + 0.5f) * stepY;
        const float pz = m_startZ + (z + 0.5f) * stepZ;

        // Determine the value by sampling from the old grid
        const float value = Sample(px, py, pz);
        
        // Write to the new grid
        values[index++] = value;
    }

    m_dimX = dimX;
    m_dimY = dimY;
    m_dimZ = dimZ;

    m_stepX = stepX;
    m_stepY = stepY;
    m_stepZ = stepZ;

    SetAll(values);
}

void Grid3D::Print() const noexcept
{
    for (int32_t z = 0; z < m_dimZ; ++z)
    {
        for (int32_t y = 0; y < m_dimY; ++y)
        {
            for (int32_t x = 0; x < m_dimX; ++x)
                std::cout << Get(x, y, z) << ", ";
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Grid3D::Draw(Renderer& renderer) const noexcept
{
    // Illustrate each element in the grid as a translucent cube
    Colormap map("inferno");
    const glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec3 scale = 4.0f * glm::vec3(m_stepX, m_stepY, m_stepZ);
    const float opacity = 0.2f;

    for (int32_t z = 0; z < m_dimZ; ++z)
    {
        for (int32_t y = 0; y < m_dimY; ++y)
        {
            for (int32_t x = 0; x < m_dimX; ++x)
            {
                const glm::vec3 position = glm::vec3(x+0.5f, y+0.5f, z+0.5f) * scale;
                const glm::vec4 colour = glm::vec4(map(Get(x, y, z)), opacity);
                renderer.drawCube(position, rotation, scale, colour);
            }
        }
    }
}

