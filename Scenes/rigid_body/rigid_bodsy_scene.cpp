
#include <random>
#include "rigid_body_scene.hpp"

void RigidBodyScene::init()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = -m_simulationSize/2; i < m_simulationSize/2; ++i)
    for (int j = -m_simulationSize/2; j < m_simulationSize/2; ++j)
    for (int k = -m_simulationSize/2; k < m_simulationSize/2; ++k)
    {
        const glm::vec3 eulerAngles = 2.0f * glm::pi<float>() *
            (glm::vec3(0.5f) - glm::vec3(dis(gen), dis(gen), dis(gen))); // [-pi, pi]
        
        m_rigidBodies.emplace_back(
            2.0f * glm::vec3(i, j, k), // position
            glm::quat(eulerAngles), // orientation
            glm::vec3(1.0f) - 2.0f * glm::vec3(dis(gen), dis(gen), dis(gen)), // linear vel [-1.0, 1.0]
            glm::vec3(0.5f) - glm::vec3(dis(gen), dis(gen), dis(gen)), // angular vel [-0.5, 0.5]
            1.0f + dis(gen), // mass [1.0, 2.0]
            1.0f, // restitution [1.0, 1.0]
            glm::vec3(0.3f) + 0.3f * glm::vec3(dis(gen), dis(gen), dis(gen)), // scale [0.3, 0.6]
            glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f) // colour
        );
    }

    m_aabb = AABB(
        glm::vec3(0.0f), // centre
        1.0f, // restitution
        3.0f * glm::vec3(m_simulationSize), // scale
        glm::vec4(1.0f) // colour
    );
}

void RigidBodyScene::simulateStep()
{
    for (RigidBody& rigidBody : m_rigidBodies)
    {
        rigidBody.ClearForces();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        const ImVec2 drag = ImGui::GetMouseDragDelta(1);
        if (drag.x != 0.0f || drag.y != 0.0f)
        {
            const float coef = 0.1f;
            const glm::vec3 dx =  drag.x * m_cameraRight;
            const glm::vec3 dy = -drag.y * m_cameraUp;
            const glm::vec3 force = (dx + dy) * coef;

            for (RigidBody& rigidBody : m_rigidBodies)
                cuboid.AddForce(force, glm::vec3(0.0f));
        }
    }
        
    if (m_gravityEnabled)
    {
        for (RigidBody& rigidBody : m_rigidBodies)
        {
            rigidBody.ApplyGravity();
        }
    }

    m_lag += ImGui::GetIO().DeltaTime;
    while (m_lag > m_stepTime) 
    { 
        for (RigidBody& rigidBody : m_rigidBodies)
        {
            rigidBody.Step(m_stepTime);
        }

        for (RigidBody& rigidBody : m_rigidBodies)
        {
            aabb.Constrain(rigidBody);
        }

        for (size_t i = 0; i < m_rigidBodies.size(); ++i)
        for (size_t j = i+1; j < m_rigidBodies.size(); ++j)
        {
            m_rigidBodies[i].Collide(m_rigidBodies[j]);
        }

        m_lag -= m_stepTime;
    }
}

void RigidBodyScene::onDraw(Renderer& renderer)
{
    m_aabb.Draw(renderer);
    for (RigidBody& rigidBody : m_rigidBodies)
    {
        rigidBody.Draw(renderer);
    }

    const glm::mat4 inverseView = glm::inverse(renderer.camera.viewMatrix);
    m_cameraRight = inverseView * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    m_cameraUp    = inverseView * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

void RigidBodyScene::onGUI()
{
    ImGui::SliderFloat("Step Time", &m_stepTime, 0.001f, 0.1f);
 
    if (ImGui::Button(m_gravityEnabled ? "GRAVITY ENABLED" : "GRAVITY DISABLED"))
    {
        m_gravityEnabled = !m_gravityEnabled;
    }
}

