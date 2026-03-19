
#include "mass_spring_scene.hpp"

void MassSpringScene::init()
{
    m_system.aabb = AABB{glm::vec3(0.0f), glm::vec3(10.0f)};

    const float length = 4.0f;
    const int size = 3;
    const float mass = 1.0f;
    const float damping = 1.0f;
    const float restitution = 0.0f;
    const float friction = 0.0f;
    const float stiffness = 300.0f;
    const float restLength = length / (size - 1);

    for (int z = -(size/2); z <= size/2; ++z)
    for (int y = -(size/2); y <= size/2; ++y)
    for (int x = -(size/2); x <= size/2; ++x)
    {
        m_system.points.emplace_back(
            restLength * glm::vec3(x, y, z), 
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 0.0f, 0.0f), 
            mass, 
            damping, 
            restitution, 
            friction);
    }

    // Connect mass points to each other with springs of length 1
    for (int i = 1; i < size*size*size; ++i) 
    {
        if (i % size != 0) 
        {
            m_system.springs.emplace_back(m_system.points[i-1], m_system.points[i], stiffness, restLength);
        }
    }
    for (int i = size; i < size*size*size; ++i) 
    {
        if (i % (size*size) >= size) 
        {
            m_system.springs.emplace_back(m_system.points[i-size], m_system.points[i], stiffness, restLength);
        }
    }
    for (int i = size*size; i < size*size*size; ++i) 
    {
        m_system.springs.emplace_back(m_system.points[i-size*size], m_system.points[i], stiffness, restLength);
    }
 
    // Connect mass points to each other diagonally with springs of length sqrt(2)
    for (int i = size; i < size*size*size; ++i) 
    {
        if (i % size != size-1 && i % (size*size) >= size) 
        {
            m_system.springs.emplace_back(m_system.points[i-(size-1)], m_system.points[i], stiffness, restLength * glm::sqrt(2.0f));
        }
    }
    for (int i = size+1; i < size*size*size; ++i) 
    {
        if (i % size != 0 && i % (size*size) > size) 
        {
            m_system.springs.emplace_back(m_system.points[i-(size+1)], m_system.points[i], stiffness, restLength * glm::sqrt(2.0f));
        }
    }
    for (int i = size*size; i < size*size*size; ++i) 
    {
        if (i % size != size-1) 
        {
            m_system.springs.emplace_back(m_system.points[i-(size*size-1)], m_system.points[i], stiffness, restLength * glm::sqrt(2.0f));
        }
    }
    for (int i = size*size+1; i < size*size*size; ++i) 
    {
        if (i % size != 0) 
        {
            m_system.springs.emplace_back(m_system.points[i-(size*size+1)], m_system.points[i], stiffness, restLength * glm::sqrt(2.0f));
        }
    }
    for (int i = size*size; i < size*size*size; ++i) 
    {
        if (i % (size*size) < size*size-size) 
        {
            m_system.springs.emplace_back(m_system.points[i-(size*size-size)], m_system.points[i], stiffness, restLength * glm::sqrt(2.0f));
        }
    }
    for (int i = size*size+size; i < size*size*size; ++i) 
    {
        if (i % (size*size) > size) 
        {
            m_system.springs.emplace_back(m_system.points[i-(size*size+size)], m_system.points[i], stiffness, restLength * glm::sqrt(2.0f));
        }
    }
}

void MassSpringScene::onDraw(Renderer& renderer)
{
    m_system.Draw(renderer);

    // Up and right directions of the camera are needed to detect mouse inputs.
    const glm::mat4 inverseView = glm::inverse(renderer.camera.viewMatrix);
    m_cameraRight = inverseView * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    m_cameraUp = inverseView * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

void MassSpringScene::simulateStep()
{
    // You can interact with the simulation with the right button of a mouse.
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        const ImVec2 drag = ImGui::GetMouseDragDelta(1);
        if (drag.x != 0.0f || drag.y != 0.0f) 
        {
            const glm::vec3 dx = drag.x * m_cameraRight;
            const glm::vec3 dy = -drag.y * m_cameraUp;
            for (MassPoint& point : _system.points) 
            {
                point.velocity += (dx + dy) * 0.01f;
            }
        }
    }

    m_lag += ImGui::GetIO().DeltaTime;
    while (m_lag > m_stepTime) 
    {
             if (m_currentMethod == 0) { m_eulerSolver   .Step(m_system, m_stepTime); } 
        else if (m_currentMethod == 1) { m_midpointSolver.Step(n_system, m_stepTime); } 
        else if (m_currentMethod == 2) { m_leapFrogSolver.Step(n_system, m_stepTime); }
        m_lag -= m_stepTime;
    }
}

void MassSpringScene::onGUI()
{
    static const char* methods[] = {"Euler Method", "Midpoint Method", "Leap Frog Method"};
    
    ImGui::SliderFloat("Step Time", &m_stepTime, 0.001f, 0.1f);
    if (ImGui::Button(m_system.isGravityEnabled ? "GRAVITY ENABLED" : "GRAVITY DISABLED")) 
    {
        m_system.isGravityEnabled = !m_system.isGravityEnabled;
    }

    // If the integration method is changed,
    // then call its initilisation function before stepping.
    int prevMethod = m_currentMethod;
    ImGui::Combo("Method", &m_currentMethod, methods, IM_ARRAYSIZE(methods));
    if (prevMethod != m_currentMethod)
    {
             if (m_currentMethod == 0) { m_eulerSolver   .Init(m_system, m_stepTime); } 
        else if (m_currentMethod == 1) { m_midpointSolver.Init(m_system, m_stepTime); } 
        else if (m_currentMethod == 2) { m_leapFrogSolver.Init(m_system, m_stepTime); }
    }
}

