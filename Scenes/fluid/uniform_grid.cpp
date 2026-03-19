
#include "uniform_grid.hpp"

glm::ivec3 UniformGrid::ComputeParticleCellCoord(const glm::vec3& position) const noexcept
{
    const glm::vec3 relativePosition = position - m_lowCorner;
    const int x = static_cast<int>(relativePosition.x / m_cellScale.x);
    const int y = static_cast<int>(relativePosition.y / m_cellScale.y);
    const int z = static_cast<int>(relativePosition.z / m_cellScale.z);
    return glm::ivec3(x, y, z);
}

int UniformGrid::ComputeCellKey(const glm::ivec3& cellCoord) const noexcept
{
    return cellCoord.x + m_partition.x * (cellCoord.y + m_partition.y * cellCoord.z);
}

int UniformGrid::PositionToCellKey(const glm::vec3& position) const noexcept
{
    const glm::ivec3 cellCoord = ComputeParticleCellCoord(position);
    const int cellKey = ComputeCellKey(cellCoord);
    return cellKey;
}

void UniformGrid::UpdateNeighbors(ParticlePool& pool) noexcept
{
    m_spatialLookup.resize(pool.size);
    for (size_t i = 0; i < pool.size; ++i)
    {
        m_spatialLookup[i] = ParticleCell{
            .cellKey = PositionToCellKey(pool.positions[i]),
            .particleIndex = static_cast<int>(i),
        };
    }

    std::sort(m_spatialLookup.begin(), m_spatialLookup.end(),
        [](const ParticleCell& a, const ParticleCell& b){ return a.cellKey < b.cellKey; });

    std::fill(m_cellIndices.begin(), m_cellIndices.end(), CellIndex{-1, -1});
    for (size_t i = 0; i < m_spatialLookup.size(); ++i)
    {
        const int cellKey = m_spatialLookup[i].cellKey;
        if (m_cellIndices[cellKey].start == -1)
        {
            m_cellIndices[cellKey].start = i;
        }
        m_cellIndices[cellKey].end = i;
    }
}

std::vector<size_t> UniformGrid::QueryNeighbors(const ParticlePool& pool, size_t particleIndex) const noexcept
{
    std::vector<size_t> neighbors;
    const glm::ivec3 cellCoord = ComputeParticleCellCoord(pool.positions[particleIndex]);
    const glm::ivec3 startOffset = {
            (cellCoord.x == 0) ? 0 : -1,
            (cellCoord.y == 0) ? 0 : -1,
            (cellCoord.z == 0) ? 0 : -1,
    };
    const glm::ivec3 endOffset = {
        (cellCoord.x == m_partition.x - 1) ? 0 : 1,
        (cellCoord.y == m_partition.y - 1) ? 0 : 1,
        (cellCoord.z == m_partition.z - 1) ? 0 : 1,
    };

    for (int dz = startOffset.z; dz <= endOffset.z; ++dz)
    {
        for (int dy = startOffset.y; dy <= endOffset.y; ++dy)
        {
            for (int dx = startOffset.x; dx <= endOffset.x; ++dx)
            {
                const glm::ivec3 neighborCellCoord = cellCoord + glm::ivec3(dx, dy, dz);
                const int neighborCellKey = ComputeCellKey(neighborCellCoord);
                const int startIndex = m_cellIndices[neighborCellKey].start;
                const int endIndex   = m_cellIndices[neighborCellKey].end;

                if (startIndex == -1) { continue; }

                for (int i = startIndex; i < endIndex; ++i)
                {
                    const size_t index = static_cast<size_t>(m_spatialLookup[i].particleIndex);
                    if (index != particleIndex) { neighbors.push_back(index); }
                }
            }
        }
    }

    return neighbors;
}

