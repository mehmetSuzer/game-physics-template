
#pragma once

#include "particle_pool.hpp"

class UniformGrid final
{
private:
    struct ParticleCell final
    {
        int cellKey;
        int particleIndex;
    };

    struct CellIndex final
    {
        int start;
        int end;
    };

    glm::vec3 m_lowCorner;
    glm::vec3 m_highCorner;
    glm::ivec3 m_partition;
    glm::vec3 m_cellScale;

    std::vector<ParticleCell> m_spatialLookup;
    std::vector<CellIndex> m_cellIndices;

    glm::ivec3 ComputeParticleCellCoord(const glm::vec3& position) const noexcept;
    int ComputeCellKey(const glm::ivec3& cellCoord) const noexcept;
    int PositionToCellKey(const glm::vec3& position) const noexcept;

public:
    UniformGrid() = default;

    UniformGrid(const glm::vec3& centre, const glm::vec3& scale, const glm::ivec3& partition) noexcept :
        m_lowCorner(centre - 0.5f * scale), m_highCorner(centre + 0.5f * scale), m_partition(partition)
    {
        m_cellScale = glm::vec3{
            scale.x / partition.x,
            scale.y / partition.y,
            scale.z / partition.z,
        };

        const size_t numCells = partition.x * partition.y * partition.z;
        m_cellIndices.resize(numCells);
    }

    void UpdateNeighbors(ParticlePool& pool) noexcept;
    std::vector<size_t> QueryNeighbors(const ParticlePool& pool, size_t particleIndex) const noexcept;

    template <class Function>
    inline void ForEachNeighbor(const ParticlePool& pool, size_t particleIndex, Function&& func) const noexcept
    {
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
                        if (index != particleIndex) { func(index); }
                    }
                }
            }
        }
    }
};

