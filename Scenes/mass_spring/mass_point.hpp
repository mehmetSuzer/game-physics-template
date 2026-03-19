
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "Renderer.h"

struct MassPoint final
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    float mass;
    float damping;
    float restitution; // 0.0f: completely ineleastic -> 1.0f: perfectly eleastic
    float friction;    // 0.0f: no horizontal velocity loss in constraint collisions-> 1.0f: all horizontal velocity is lost
   
    inline void Draw(Renderer& renderer) const noexcept
    {
        constexpr float RADIUS = 0.1f;
        constexpr glm::vec4 COLOUR = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        renderer.drawSphere(position, RADIUS, COLOUR);
    }

    // You can print the state of the MassPoint as follows:
    // std::cout << point << std::endl;
    friend inline std::ostream& operator<<(std::ostream& os, const MassPoint& point)
    {
        const glm::vec3& pos = point.position;
        const glm::vec3& vel = point.velocity;

        os << "pos = vec3(" << pos.x << ", " << pos.y << ", " << pos.z << ") | "
           << "vel = vec3(" << vel.x << ", " << vel.y << ", " << vel.z << ")";
        return os;
    }
};

