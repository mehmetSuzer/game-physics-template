
#include "mass_spring_system.hpp"

void MassSpringSystem::UpdateForces() noexcept
{
    for (MassPoint& point : points)
    {
        point.force = -point.damping * point.velocity;
        if (isGravityEnabled) 
        {
            constexpr glm::vec3 GRAVITY = glm::vec3(0.0f, 0.0f, -9.81f);
            point.force += GRAVITY;
        }
    }

    for (Spring& spring : springs)
    {
        const glm::vec3 elasticForce = spring.GetForce();
        spring.point1.force += elasticForce;
        spring.point2.force -= elasticForce;
    }
}

void MassSpringSystem::ApplyConstraint() noexcept
{
    for (MassPoint& point : points)
    {
        aabb.Constrain(point);
    }
}

void MassSpringSystem::Draw(Renderer& renderer) const noexcept
{
    aabb.Draw(renderer);
    for (const MassPoint& point : points)
    {
        point.Draw(renderer);
    }
    for (const Spring& spring : springs)
    {
        spring.Draw(renderer);
    }
}

void MassSpringSystem::PrintState() const noexcept
{
    for (size_t i = 0; i < _points.size(); ++i)
    {
        std::cout << "Mass Point " << i << ": " << points[i] << "\n";
    }
    std::cout << "\n";
}

