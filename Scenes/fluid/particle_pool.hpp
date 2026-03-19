
#pragma once

#include "Renderer.h"

struct ParticlePool final
{
private:
    static constexpr float BLUE_SPEED  = 0.0f;
    static constexpr float GREEN_SPEED = 2.0f;
    static constexpr float RED_SPEED   = 4.0f;

    glm::vec4 GetColour(size_t index) const noexcept;

public:
    static constexpr float PARTICLE_RADIUS = 0.1f;
    static constexpr float PARTICLE_MASS   = 1.0f;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> velocities;
    std::vector<glm::vec3> forces;
    std::vector<float> densities;
    std::vector<float> pressures;

    size_t size = 0;

    void Resize(size_t n) noexcept;
    void Draw(Renderer& renderer) const noexcept;
};

