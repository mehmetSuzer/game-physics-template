
#include "pde_scene2.hpp"

void PDEScene2::init()
{
    const float startX = 0.0f;
    const float startY = 0.0f;
    const float startZ = 0.0f;

    const float endX = 1.0f;
    const float endY = 1.0f;
    const float endZ = 1.0f;

    m_grid = Grid3D(m_dimX, m_dimY, m_dimZ, startX, startY, startZ, endX, endY, endZ);

    const float low = 0.3f;
    const float high = 1.0f;

    m_initialiser.SmoothZ(m_grid, low, high);
}

void PDEScene2::simulateStep()
{
    if (m_paused) { return; }

    // You can change the temperature of the middle cells by pressing Space 
    if (ImGui::IsKeyDown(ImGuiKey_Space))
    {
        // If you press Space by holding "-", then the new temperature of the cells is -10.0f
        // Otherwise, it is 10.0f
        const float value = (ImGui::IsKeyDown(ImGuiKey_Minus)) ? -10.0f : 10.0f;

        m_grid.Set(m_dimX/2,   m_dimY/2,   m_dimZ/2,   value);
        m_grid.Set(m_dimX/2,   m_dimY/2,   m_dimZ/2+1, value);
        m_grid.Set(m_dimX/2,   m_dimY/2+1, m_dimZ/2,   value);
        m_grid.Set(m_dimX/2,   m_dimY/2+1, m_dimZ/2+1, value);
        m_grid.Set(m_dimX/2+1, m_dimY/2,   m_dimZ/2,   value);
        m_grid.Set(m_dimX/2+1, m_dimY/2,   m_dimZ/2+1, value);
        m_grid.Set(m_dimX/2+1, m_dimY/2+1, m_dimZ/2,   value);
        m_grid.Set(m_dimX/2+1, m_dimY/2+1, m_dimZ/2+1, value);
    }

    m_lag += ImGui::GetIO().DeltaTime;
    while (m_lag > m_stepTime) 
    {
             if (m_method == 0) { m_explicitSolver.Step(m_grid, m_simulationSpeed * m_stepTime, m_diffusivity); }
        else if (m_method == 1) { m_implicitSolver.Step(m_grid, m_simulationSpeed * m_stepTime, m_diffusivity); }

        m_lag -= m_stepTime;
    }
}

void PDEScene2::onDraw(Renderer& renderer)
{
    // Since the 3D grid is visualised with transparent cubes, we have to enable depth sorthing
    // so that the renderer can render cubes from far to near
    renderer.enableDepthSorting();
    m_grid.Draw(renderer);
}

void PDEScene2::onGUI()
{
    ImGui::SliderFloat("Step Time", &m_stepTime, 0.001f, 0.1f);
    ImGui::SliderFloat("Diffusivity", &m_diffusivity, 0.0f, 0.5f);
    ImGui::SliderFloat("Simuation Speed", &m_simulationSpeed, 0.0f, 1.0f);

    if (ImGui::Button(m_paused ? "PLAY" : "PAUSE"))
    {
        m_paused = !m_paused;
    }

    if (ImGui::Button("PRINT"))
    {
        m_grid.Print();
    }

    static const char* methods[] = {"Explicit Integration", "Implicit Integration"};
    ImGui::Combo("Method", &m_method, methods, IM_ARRAYSIZE(methods));

    if (ImGui::SliderInt("Dim X", &m_dimX, 4, 32) || 
        ImGui::SliderInt("Dim Y", &m_dimY, 4, 32) ||
        ImGui::SliderInt("Dim Z", &m_dimZ, 4, 32))
    {
        m_grid.Rescale(m_dimX, m_dimY, m_dimZ);
    }
}

