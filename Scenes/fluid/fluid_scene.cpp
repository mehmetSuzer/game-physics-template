
#include "glm/gtx/string_cast.hpp"
#include "fluid_scene.hpp"

void FluidScene::init()
{
    constexpr int32_t numParticles = SIZE * SIZE * SIZE;
    constexpr float particleSpacing = 2.0f * ParticlePool::PARTICLE_RADIUS + SPACING;

    constexpr glm::vec3 containerCentre = glm::vec3(0.0f);
    constexpr glm::vec3 containerScale = glm::vec3(0.4f * SIZE);
    constexpr float damping = 0.8f;

    constexpr glm::ivec3 gridPartition = {
        static_cast<int>(containerScale.x / Kernel::RADIUS),
        static_cast<int>(containerScale.y / Kernel::RADIUS),
        static_cast<int>(containerScale.z / Kernel::RADIUS),
    };

    m_container = Container(containerCentre, containerScale, damping);
    m_grid = UniformGrid(containerCentre, containerScale, gridPartition);
    m_solver = Solver<Kernel>(numParticles);

    int32_t i = 0;
    m_pool.Resize(numParticles);

    for (int32_t z = -SIZE/2; z < (SIZE+1)/2; ++z)
    {
        for (int32_t y = -SIZE/2; y < (SIZE+1)/2; ++y)
        {
            for (int32_t x = -SIZE/2; x < (SIZE+1)/2; ++x)
            {
                m_pool.positions[i++] = particleSpacing * glm::vec3(x, y, z);
            }
        }
    }
}

void FluidScene::simulateStep()
{
    if (m_paused) { return; }

    m_lag += ImGui::GetIO().DeltaTime;
    while (m_lag > m_stepTime) 
    {
        m_grid.UpdateNeighbors(m_pool);
        m_solver.Step(m_pool, m_grid, m_stepTime);
        m_container.Bound(m_pool);
        m_lag -= m_stepTime;
    }
}

void FluidScene::onDraw(Renderer& renderer)
{
    m_container.Draw(renderer);
    m_pool.Draw(renderer);
}

void FluidScene::onGUI()
{
    ImGui::SliderFloat("Step Time", &m_stepTime, 0.001f, 0.1f);
    if (ImGui::Button(m_paused ? "PLAY" : "PAUSE"))
    {
        m_paused = !m_paused;
    }
}

