
#pragma once

#include "mass_point.hpp"

struct Spring final
{    
    MassPoint& point1;
    MassPoint& point2;
    float stiffness;
    float restLength;

    inline glm::vec3 GetForce() const noexcept
    {
        const glm::vec3 difference = point2.position - point1.position;
        const float length = glm::length(difference);
        const glm::vec3 direction = difference / length;
        const float magnitude = stiffness * (length - restLength);
        const glm::vec3 force = magnitude * direction;
        return force;
    }

    inline void Draw(Renderer& renderer) const noexcept
    {
        constexpr glm::vec4 COLOUR = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        renderer.drawLine(point1.position, point2.position, COLOUR);
    }
};

