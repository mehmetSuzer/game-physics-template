
#include "pde_scene1.hpp"

void PDEScene1::init()
{   
    const float startX = 0.0f;
    const float startY = 0.0f;

    const float endX = 1.0f;
    const float endY = 1.0f;

    m_grid = Grid2D(m_dimX, m_dimY, startX, startY, endX, endY);

    const float mean = 0.0f;
    const float std = 2.0f;

    m_initialiser.Normal(m_grid, mean, std);
}

void PDEScene1::simulateStep()
{
    if (m_paused) { return; }

    // You can change the temperature of a cell by right-clicking on it
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        const ImVec2 mousePos = ImGui::GetMousePos();
        
        const int32_t x = static_cast<int32_t>(std::floor(mousePos.x * m_dimX / displaySize.x));
        const int32_t y = static_cast<int32_t>(std::floor(mousePos.y * m_dimY / displaySize.y));
    
        // If you right-click while the minus key "-" is hold, then the new temperature of the cell is -1.0f
        // Otherwise, it is 1.0f
        const float value = (ImGui::IsKeyDown(ImGuiKey_Minus)) ? -1.0f : 1.0f;
        m_grid.Set(x, y, value);
    }

    m_lag += ImGui::GetIO().DeltaTime;
    while (m_lag > m_stepTime) 
    {
        // Apply the integration method of your choice
             if (m_method == 0) { m_explicitSolver.Step(m_grid, m_simulationSpeed * m_stepTime, m_diffusivity); }
        else if (m_method == 1) { m_implicitSolver.Step(m_grid, m_simulationSpeed * m_stepTime, m_diffusivity); }

        m_lag -= m_stepTime;
    }
}

void PDEScene1::onDraw(Renderer& renderer)
{
    m_grid.Draw(renderer);
}

void PDEScene1::onGUI()
{
    ImGui::SliderFloat("Step Time", &m_stepTime, 0.001f, 0.1f);
    ImGui::SliderFloat("Diffusivity", &m_diffusivity, 0.0f, 0.5f);
    ImGui::SliderFloat("Simulation Speed", &m_simulationSpeed, 0.0f, 1.0f);

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

    // Once you change the X and Y dimensions, the grid is rescaled accordingly
    if (ImGui::SliderInt("Dim X", &m_dimX, 4, 64) || ImGui::SliderInt("Dim Y", &m_dimY, 4, 64))
    {
        m_grid.Rescale(m_dimX, m_dimY);
    }
}

