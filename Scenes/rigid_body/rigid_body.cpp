
#include "rigid_body.h"

RigidBody::RigidBody(
    const glm::vec3& centreOfMass, 
    const glm::quat& orientation,
    const glm::vec3& linearVelocity,
    const glm::vec3& angularVelocity,
    float mass,
    float restitution,
    const glm::vec3& scale,
    const glm::vec4& colour) noexcept : 
    m_centreOfMass(centreOfMass),
    m_orientation(glm::normalize(orientation)),
    m_linearVelocity(linearVelocity),
    m_angularVelocity(angularVelocity),
    m_inverseMass(1.0f / mass),
    m_restitution(restitution),
    m_scale(scale),
    m_colour(colour) 
{
    const float width  = m_scale.x;
    const float depth  = m_scale.y;
    const float height = m_scale.z;
 
    m_inverseBaseInertiaTensor = (12.0f * m_inverseMass) * glm::mat3(
        1.0f / (height*height + depth*depth), 0.0f, 0.0f,
        0.0f, 1.0f / (width*width + height*height), 0.0f,
        0.0f, 0.0f, 1.0f / (width*width + depth*depth));

    const glm::mat3 rotation = mat3_cast(m_orientation);
    m_inverseInertiaTensor = rotation * m_inverseBaseInertiaTensor * glm::transpose(rotation);
    m_angularMomentum = glm::inverse(m_inverseInertiaTensor) * angularVelocity;
}

glm::vec3 RigidBody::GetWorldSpacePositionAt(const glm::vec3& leverArm) const noexcept
{
    return m_centreOfMass + leverArm;
}

glm::vec3 RigidBody::GetWorldSpaceVelocityAt(const glm::vec3& leverArm) const noexcept
{
    return m_linearVelocity + glm::cross(m_angularVelocity, leverArm);
}

glm::mat4 RigidBody::GetModelMatrix() const noexcept
{
    glm::mat4 model = glm::mat4(1.0f); // I
    model = glm::translate(model, m_centreOfMass); // I * T = T
    model = model * glm::mat4_cast(m_orientation); // T * R = TR
    model = glm::scale(model, m_scale); // TR * S = TRS
    return model;
}

void RigidBody::ClearForces() noexcept
{
    m_totalForce  = glm::vec3(0.0f, 0.0f, 0.0f);
    m_totalTorque = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::AddForce(const glm::vec3& force, const glm::vec3& leverArm) noexcept
{
    m_totalForce += force;
    m_totalTorque += glm::cross(leverArm, force);
}

void RigidBody::ApplyGravity() noexcept
{
    constexpr glm::vec3 gravity = glm::vec3(0.0f, 0.0f, -2.0f);
    if (m_inverseMass <= 0.0f) return;
    m_totalForce += gravity * (1.0f / m_inverseMass);
}
 
void RigidBody::Stop() noexcept
{
    ClearForces();
    m_linearVelocity  = glm::vec3(0.0f, 0.0f, 0.0f);
    m_angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    m_angularMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::Step(float stepTime) noexcept
{
    m_centreOfMass += stepTime * m_linearVelocity;
    
    const glm::vec3 linearAcceleration = m_totalForce * m_inverseMass;
    m_linearVelocity += stepTime * linearAcceleration;
 
    const glm::quat angVelQuat = glm::quat(0.0f, m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z);
    const glm::quat orientationVelocity = 0.5f * angVelQuat * m_orientation;
    m_orientation += stepTime * orientationVelocity;
    m_orientation = glm::normalize(m_orientation);

    m_angularMomentum += stepTime * m_totalTorque;

    const glm::mat3 rotation = mat3_cast(m_orientation);
    m_inverseInertiaTensor = rotation * m_inverseBaseInertiaTensor * glm::transpose(rotation);

    m_angularVelocity = m_inverseInertiaTensor * m_angularMomentum;
}

void RigidBody::ApplyImpulse(const glm::vec3& impulse, const glm::vec3& leverArm) noexcept
{
    m_linearVelocity += impulse * m_inverseMass;
    m_angularMomentum += glm::cross(leverArm, impulse);
}

void RigidBody::Collide(RigidBody& other) noexcept
{
    glm::mat4 selfModelMatrix = GetModelMatrix();
    glm::mat4 otherModelMatrix = other.GetModelMatrix();
    const CollisionInfo info = collisionTools::checkCollisionSAT(selfModelMatrix, otherModelMatrix);

    if (!info.isColliding) { return; }

    const glm::vec3 selfLeverArm = info.collisionPointWorld - m_centreOfMass;
    const glm::vec3 selfVelocity = GetWorldSpaceVelocityAt(selfLeverArm);

    const glm::vec3 otherLeverArm = info.collisionPointWorld - other._positionOfCentreOfMass;
    const glm::vec3 otherVelocity = other.GetWorldSpaceVelocityAt(otherLeverArm);

    // Impulse in the Normal Direction
    const glm::vec3 relativeVelocity = selfVelocity - otherVelocity;
    const float relativeNormalMagnitude = glm::dot(relativeVelocity, info.normalWorld);

    if (relativeNormalMagnitude >= 0.0f) { return; }

    const float denom = m_inverseMass + other.m_inverseMass + glm::dot(
        glm::cross(m_inverseInertiaTensor * glm::cross(selfLeverArm, info.normalWorld), selfLeverArm) +
        glm::cross(other.m_inverseInertiaTensor * glm::cross(otherLeverArm, info.normalWorld), otherLeverArm),
        info.normalWorld);

    if (denom <= 1e-6f) { return; }

    const float combinedRestitution = _restitution * other.m_restitution;
    const float magnitude = -(1.0f + combinedRestitution) * relativeNormalMagnitude / denom;
    const glm::vec3 impulse = magnitude * info.normalWorld;
 
    ApplyImpulse(impulse, selfLeverArm);
    other.ApplyImpulse(-impulse, otherLeverArm);
}

void RigidBody::Draw(Renderer& renderer) const noexcept
{
    renderer.drawCube(m_centreOfMass, m_orientation, m_scale, m_colour);
}

