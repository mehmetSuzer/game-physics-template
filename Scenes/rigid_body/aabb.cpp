
#include "aabb.h"
    
AABB::AABB(
    const glm::vec3& centre, 
    float restitution,
    const glm::vec3& scale,
    const glm::vec4& colour) noexcept : 
    m_centre(centre),
    m_scale(scale),
    m_colour(colour)
{
    const glm::vec3 xOffset = glm::vec3(0.5f * (m_scale.x + WALL_THICKNESS), 0.0f, 0.0f);
    m_walls.emplace_back(
        m_centre + xOffset, // position of the centre of mass
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // orientation
        glm::vec3(0.0f), // linear velocity
        glm::vec3(0.0f), // angular velocity
        MASS,
        restitution,
        glm::vec3(WALL_THICKNESS, m_scale.y + 2.0f * WALL_THICKNESS, m_scale.z), // scale
        colour);
    m_walls.emplace_back(
        m_centre - xOffset, // position of the centre of mass
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // orientation
        glm::vec3(0.0f), // linear velocity
        glm::vec3(0.0f), // angular velocity
        MASS,
        restitution,
        glm::vec3(WALL_THICKNESS, m_scale.y + 2.0f * WALL_THICKNESS, m_scale.z), // scale
        colour);
 
    const glm::vec3 yOffset = glm::vec3(0.0f, 0.5f * (m_scale.y + WALL_THICKNESS), 0.0f);
    m_walls.emplace_back(
        m_centre + yOffset, // position of the centre of mass
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // orientation
        glm::vec3(0.0f), // linear velocity
        glm::vec3(0.0f), // angular velocity
        MASS,
        restitution,
        glm::vec3(m_scale.x, WALL_THICKNESS, m_scale.z), // scale
        colour);
    m_walls.emplace_back(
        m_centre - yOffset, // position of the centre of mass
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // orientation
        glm::vec3(0.0f), // linear velocity
        glm::vec3(0.0f), // angular velocity
        MASS,
        restitution,
        glm::vec3(m_scale.x, WALL_THICKNESS, m_scale.z), // scale
        colour);

     const glm::vec3 zOffset = glm::vec3(0.0f, 0.0f, 0.5f * (m_scale.z + WALL_THICKNESS));
     m_walls.emplace_back(
        m_centre + zOffset, // position of the centre of mass
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // orientation
        glm::vec3(0.0f), // linear velocity
        glm::vec3(0.0f), // angular velocity
        MASS,
        restitution,
        glm::vec3(m_scale.x + 2.0f * WALL_THICKNESS, m_scale.y + 2.0f * WALL_THICKNESS, WALL_THICKNESS), // scale
        colour);
     m_walls.emplace_back(
        m_centre - zOffset, // position of the centre of mass
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // orientation
        glm::vec3(0.0f), // linear velocity
        glm::vec3(0.0f), // angular velocity
        MASS,
        restitution,
        glm::vec3(_scale.x + 2.0f * WALL_THICKNESS, m_scale.y + 2.0f * WALL_THICKNESS, WALL_THICKNESS), // scale
        colour);
}

void AABB::Constrain(RigidBody& rigidBody) noexcept
{
    for (RigidBody& wall : m_walls)
    {
        wall.Collide(rigidBody);
    }
}

void AABB::Draw(Renderer& renderer) const noexcept
{ 
    renderer.drawWireCube(m_centre, m_scale, m_colour);
}

