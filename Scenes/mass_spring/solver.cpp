
#include "solver.hpp"

void EulerSolver::Init(MassSpringSystem& system, float deltaTime) const noexcept
{
   // The Euler Method does not have any initialisation procedure.
   // You can directly start stepping. 
}

void EulerSolver::Step(MassSpringSystem& system, float deltaTime) const noexcept
{
    system.UpdateForces();
    for (MassPoint& point : system.points)
    {
        const glm::vec3 acceleration = point.force / point.mass;
        point.position += deltaTime * point.velocity;
        point.velocity += deltaTime * acceleration;
    }
    system.ApplyConstraint();
}

void LeapFrogSolver::Init(MassSpringSystem& system, float deltaTime) const noexcept
{
    // The velocity of an object lags half a deltaTime in the Leap Frog Method.
    // When we calculate x(t + dt) by using x(t), we calculate v(t + dt/2) by using v(t - dt/2).
    const float halfDeltaTime = 0.5f * deltaTime;
    system.UpdateForces();
    for (MassPoint& point : system.points)
    {
        const glm::vec3 acceleration = point.force / point.mass;
        point.velocity -= acceleration * halfDeltaTime; // make velocity lag half a deltaTime
    }
}

void LeapFrogSolver::Step(MassSpringSystem& system, float deltaTime) const noexcept
{
    system.UpdateForces();
    for (MassPoint& point : system.points)
    {
        const glm::vec3 acceleration = point.force / point.mass;
        point.velocity += acceleration * deltaTime; // v(t + dt/2) <- v(t - dt/2) + dt * a(t)
        point.position += point.velocity * deltaTime; // x(t + dt) <- x(t) + dt * v(t + dt/2)
    }
    system.ApplyConstraint();
}

void MidpointSolver::Init(MassSpringSystem& system, float deltaTime) const noexcept
{
    // The Midpoint Method does not have any initilisation procedure.
    // You can directly start stepping.
}

void MidpointSolver::Step(MassSpringSystem& system, float deltaTime) const noexcept
{
    const float halfDeltaTime = 0.5f * deltaTime;
    const size_t numPoints = system.points.size();
    
    std::vector<glm::vec3> currentPositions(numPoints);
    std::vector<glm::vec3> midVelocities(numPoints);

    // Update the positions by taking a half step
    // because we need to calculate the acceleration at t + dt/2
    // x(t + dt/2) <- x(t) + dt/2 * v(t)
    // We need to save the current position and the mid point velocity
    system.UpdateForces();
    for (size_t i = 0; i < numPoints; ++i)
    {
        const glm::vec3 acceleration = system.points[i].force / system.points[i].mass;
        currentPositions[i] = system.points[i].position;
        system.points[i].position += halfDeltaTime * system.points[i].velocity;
        midVelocities[i] = system.points[i].velocity + halfDeltaTime * acceleration; // v(t + dt/2) <- v(t) + dt/2 * a(t)
    }

    system.UpdateForces(); // Now forces are updated and we can calculate a(t + dt/2)
    for (size_t i = 0; i < numPoints; ++i)
    {
        const glm::vec3 acceleration = system.points[i].force / system.points[i].mass;
        system.points[i].position = currentPositions[i] + deltaTime * midVelocities[i]; // x(t + dt) <- x(t) + dt * v(t + dt/2)
        system.points[i].velocity += deltaTime * acceleration; // v(t+dt) <- v(t) + dt * a(t + dt/2)
    } 
    system.ApplyConstraint();
}

