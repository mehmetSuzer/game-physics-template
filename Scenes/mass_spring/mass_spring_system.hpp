
#pragma once

#include <vector>
#include "spring.h"
#include "aabb.hpp"

struct MassSpringSystem final
{
    std::vector<MassPoint> points;
    std::vector<Spring> springs;
    AABB aabb;
    bool isGravityEnabled = false;

    void UpdateForces() noexcept;
    void ApplyConstraint() noexcept;
    void Draw(Renderer& renderer) const noexcept;
    void PrintState() const noexcept;
};

