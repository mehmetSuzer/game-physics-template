
#pragma once

#include "kernel.hpp"
#include "particle_pool.hpp"
#include "uniform_grid.hpp"

template<class Kernel>
class Solver final
{
private:
    static constexpr glm::vec3 GRAVITY = glm::vec3(0.0f, 0.0f, -1.0f);

    float m_restDensity;
    float m_pressureStiffness;
    float m_pressureGamma;

    std::vector<float> m_pressureInfluences;

    void UpdateDensities(ParticlePool& pool, const UniformGrid& grid) const noexcept
    {
        #pragma omp parallel for
        for (size_t i = 0; i < pool.size; ++i)
        {
            pool.densities[i] = ParticlePool::PARTICLE_MASS * Kernel::ZERO_DISTANCE_W;
        }
        // #pragma omp parallel for
        for (size_t i = 0; i < pool.size; ++i)
        {
            grid.ForEachNeighbor(pool, i, [&](size_t j) {
                if (i > j) { return; }
                const glm::vec3 difference = pool.positions[i] - pool.positions[j];
                const float distance2 = glm::dot(difference, difference);
                if (distance2 >= Kernel::RADIUS * Kernel::RADIUS) { return; }
                const float influence = ParticlePool::PARTICLE_MASS * Kernel::W(distance2);
                pool.densities[i] += influence;
                pool.densities[j] += influence;
            });
        }
    }

    void UpdatePressures(ParticlePool& pool) const noexcept
    {
        #pragma omp parallel for
        for (size_t i = 0; i < pool.size; ++i)
        {
            const float ratio = pool.densities[i] / m_restDensity;
            const float pressure = m_pressureStiffness * (glm::pow(ratio, m_pressureGamma) - 1.0f);
            pool.pressures[i] = pressure;
        }
    }

    void UpdateForces(ParticlePool& pool, const UniformGrid& grid) noexcept
    {
        #pragma omp parallel for
        for (size_t i = 0; i < pool.size; ++i)
        {
            pool.forces[i] = glm::vec3(0.0f);
        }
        #pragma omp parallel for
        for (size_t i = 0; i < pool.size; ++i)
        {
            m_pressureInfluences[i] = pool.pressures[i] / (pool.densities[i] * pool.densities[i]);
        }

        // #pragma omp parallel for
        for (size_t i = 0; i < pool.size; ++i)
        {
            grid.ForEachNeighbor(pool, i, [&](size_t j) {
                if (i > j) { return; }
                const glm::vec3 difference = pool.positions[i] - pool.positions[j];
                const float distance2 = glm::dot(difference, difference);
                if (distance2 < 1e-5f || distance2 >= Kernel::RADIUS * Kernel::RADIUS) { return; }
                const float distance = glm::sqrt(distance2);
                const glm::vec3 direction = difference / distance;
                const glm::vec3 gradient = direction * Kernel::WDrv(distance2);
                const glm::vec3 influence = ParticlePool::PARTICLE_MASS * (m_pressureInfluences[i] + m_pressureInfluences[j]) * gradient;
                pool.forces[i] -= influence;
                pool.forces[j] += influence;
            });
        }
    }

    void IntegrateSemiImplicitEuler(ParticlePool& pool, float stepTime) const noexcept
    {
        #pragma omp parallel for
        for (size_t i = 0; i < pool.size; ++i)
        {
            const glm::vec3 acceleration = GRAVITY + pool.forces[i] / ParticlePool::PARTICLE_MASS;
            pool.velocities[i] += acceleration * stepTime;
            pool.positions[i] += pool.velocities[i] * stepTime;
        }
    }

public:
    Solver() = default;

    Solver(size_t numParticles, float restDensity =  12.3f, float pressureStiffness = 14.3f, float pressureGamma = 3.0f) : 
        m_restDensity(restDensity), m_pressureStiffness(pressureStiffness), m_pressureGamma(pressureGamma)
    {
        m_pressureInfluences.resize(numParticles);
    }

    void Step(ParticlePool& pool, const UniformGrid& grid, float stepTime) noexcept
    {
        UpdateDensities(pool, grid);
        UpdatePressures(pool);
        UpdateForces(pool, grid);
        IntegrateSemiImplicitEuler(pool, stepTime);
    }
};

