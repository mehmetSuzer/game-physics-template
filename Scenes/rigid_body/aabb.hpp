
#pragma once

#include <limits>
#include <vector>
#include "rigid_body.h"

class AABB final
{
private:
    static constexpr float WALL_THICKNESS = 2.0f;
    static constexpr float MASS = std::numeric_limits<float>::max();

    glm::vec3 m_centre;
    glm::vec3 m_scale;
    glm::vec4 m_colour;

    std::vector<Cuboid> m_walls;

public:
    AABB() = default;

    AABB(
        const glm::vec3& centre, 
        float restitution = 1.0f,
        const glm::vec3& scale = glm::vec3(1.0f),
        const glm::vec4& colour = glm::vec4(1.0f)) noexcept;

    void Constrain(RigidBody& rigidBody) noexcept; 
    void Draw(Renderer& renderer) const noexcept;
};

