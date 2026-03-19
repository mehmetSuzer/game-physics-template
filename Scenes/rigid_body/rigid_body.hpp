
#pragma once

#include <iostream>
#include "Renderer.h"
#include "util/CollisionDetection.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

class RigidBody final
{
private:
    glm::vec3 m_totalForce = glm::vec3(0.0f, 0.0f, 0.0f); // F
    glm::vec3 m_totalTorque = glm::vec3(0.0f, 0.0f, 0.0f); // T

    glm::vec3 m_centreOfMass; // x_cm
    glm::quat m_orientation; // q
    glm::vec3 m_linearVelocity; // v_cm
    glm::vec3 m_angularVelocity; // w
    glm::vec3 m_angularMomentum; // L
    glm::mat3 m_inverseBaseInertiaTensor; // (I_0)^(-1)
    glm::mat3 m_inverseInertiaTensor; // I^(-1)
    float m_inverseMass; // 1.0 / m
    float m_restitution; // c

    glm::vec3 m_scale; // How much a rigid body extends in x, y, and z directions
    glm::vec4 m_colour;

public:
    RigidBody() = default;

    RigidBody(
        const glm::vec3& centreOfMass, 
        const glm::quat& orientation,
        const glm::vec3& linearVelocity,
        const glm::vec3& angularVelocity,
        float mass,
        float restitution = 1.0f,
        const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f),
        const glm::vec4& colour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)) noexcept;

    // leverArm = world position of a point - world position of the centre of mass
    glm::vec3 GetWorldSpacePositionAt(const glm::vec3& leverArm) const noexcept;
    glm::vec3 GetWorldSpaceVelocityAt(const glm::vec3& leverArm) const noexcept;
    glm::mat4 GetModelMatrix() const noexcept;

    void ClearForces() noexcept;
    void AddForce(const glm::vec3& force, const glm::vec3& leverArm) noexcept;
    void ApplyGravity() noexcept;

    void Stop() noexcept;
    void Step(float stepTime) noexcept;
    void ApplyImpulse(const glm::vec3& impulse, const glm::vec3& leverArm) noexcept;
    void Collide(RigidBody& other) noexcept; 
    void Draw(Renderer& renderer) const noexcept;
    
    // You can print the state of a rigid body as follows:
    // std::cout << rigidBody << std::endl;
    // Note that glm matrices are column-wise ordered not row-wise.
    friend inline std::ostream& operator<<(std::ostream& os, const RigidBody& rigidBody) noexcept
    {
        os << "the State of the Rigid Body:\n"
           << "Position of the Centre of Mass:" << glm::to_string(rigidBody._positionOfCentreOfMass)    << "\n"
           << "Orientation                   :" << glm::to_string(rigidBody._orientation)               << "\n"
           << "Linear Velocity               :" << glm::to_string(rigidBody._linearVelocity)            << "\n"
           << "Angular Velocity              :" << glm::to_string(rigidBody._angularVelocity)           << "\n"
           << "Inverse Base Inertia Tensor   :" << glm::to_string(rigidBody._inverseBaseInertiaTensor)  << "\n"
           << "Inverse Inertia Tensor        :" << glm::to_string(rigidBody._inverseInertiaTensor)      << "\n"
           << "Inverse Mass                  :" <<                rigidBody._inverseMass                << "\n"
           << "Total Force                   :" << glm::to_string(rigidBody._totalForce)                << "\n"
           << "Total Torque                  :" << glm::to_string(rigidBody._totalTorque)               << "\n";
           
        return os;
    }
};

