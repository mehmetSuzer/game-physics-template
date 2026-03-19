
#pragma once

#include "mass_point.hpp"

struct AABB final
{
    glm::vec3 centre;
    glm::vec3 size;

    inline void Constrain(MassPoint& point) const noexcept
    {
        const glm::vec3 halfSize = 0.5f * size;
        const glm::vec3 min = centre - halfSize;
        const glm::vec3 max = centre + halfSize;

        glm::vec3 closestPoint = {
            glm::clamp(point.position.x, min.x, max.x),
            glm::clamp(point.position.y, min.y, max.y),
            glm::clamp(point.position.z, min.z, max.z)};

        const glm::vec3 difference = point.position - closestPoint;
        const float distance = glm::length(difference);

        if (distance < point.RADIUS) { return; }

        const glm::vec3 normal = (distance > 1E-5f) ? difference / distance : glm::normalize(centre - point.position);
        const float penetration = point.RADIUS - distance;
        point.position += normal * penetration;

        const glm::vec3 perpendicularVelocity = glm::dot(point.velocity, normal) * normal;
        point.velocity -= (1.0f + point.restitution) * perpendicularVelocity;

        const glm::vec3 tangentVelocity = point.velocity - perpendicularVelocity;
        point.velocity -= point.friction * tangentVelocity;
    }

    inline void Draw() const noexcept
    {
        constexpr glm::vec3 COLOUR = glm::vec3(1.0f, 1.0f, 1.0f);
        renderer.drawWireCube(centre, size, COLOUR);
    }
};

