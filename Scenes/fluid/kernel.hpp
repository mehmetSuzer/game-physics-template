
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// max(radius - distance, 0.0)
class MaxKernel final
{
public:
    static constexpr float RADIUS = 1.0f;

private:
    static constexpr float INV_VOLUME = 3.0f / (glm::pi<float>() * RADIUS * RADIUS * RADIUS * RADIUS);

public:
    static constexpr float ZERO_DISTANCE_W = INV_VOLUME * RADIUS;
    static constexpr float ZERO_DISTANCE_W_DRV = -INV_VOLUME;

    static inline float W(float distance2) noexcept
    {
        const float factor = RADIUS - glm::sqrt(distance2);
        return INV_VOLUME * factor;   
    }

    static inline float WDrv(float distance2) noexcept
    {
        return -INV_VOLUME;
    }
};

// max((radius - distance)^3, 0.0)
class MaxCubeKernel final
{
public:
    static constexpr float RADIUS = 1.0f;

private:
    static constexpr float INV_VOLUME = 15.0f / (glm::pi<float>() * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS);
    
public:
    static constexpr float ZERO_DISTANCE_W = INV_VOLUME * RADIUS * RADIUS * RADIUS;
    static constexpr float ZERO_DISTANCE_W_DRV = -3.0f * INV_VOLUME * RADIUS * RADIUS;

    static inline float W(float distance2) noexcept
    {
        const float factor = RADIUS - glm::sqrt(distance2);
        return INV_VOLUME * factor * factor * factor;
    }

    static inline float WDrv(float distance2) noexcept
    {
        const float factor = RADIUS - glm::sqrt(distance2);
        return -3.0f * INV_VOLUME * factor * factor;
    }
};

// max((radius^2 - distance^2)^3, 0.0)
class Poly6Kernel final
{
public:
    static constexpr float RADIUS = 1.0f;

private:
    static constexpr float INV_VOLUME = 315.0f / (64.0f * glm::pi<float>() * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS);

public:
    static constexpr float ZERO_DISTANCE_W = INV_VOLUME * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS;
    static constexpr float ZERO_DISTANCE_W_DRV = 0.0f;

    static inline float W(float distance2) noexcept
    {
        const float factor = RADIUS * RADIUS - distance2;
        return INV_VOLUME * factor * factor * factor;
    }

    static inline float WDrv(float distance2) noexcept
    {
        const float distance = glm::sqrt(distance2);
        const float factor = RADIUS * RADIUS - distance2;
        return -6.0f * INV_VOLUME * distance * factor * factor;
    }
};

// max((radius - distance)^4 * (radius + 4 * distance), 0.0)
class WendlandC2Kernel final
{
public:
    static constexpr float RADIUS = 1.0f;

private:
    static constexpr float INV_VOLUME = 21.0f / (2.0f * glm::pi<float>() * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS);

public:
    static constexpr float ZERO_DISTANCE_W = INV_VOLUME * RADIUS * RADIUS * RADIUS * RADIUS * RADIUS;
    static constexpr float ZERO_DISTANCE_W_DRV = 0.0f;

    static inline float W(float distance2) noexcept
    {
        const float distance = glm::sqrt(distance2);
        const float factor = RADIUS - distance;
        const float factor2 = factor * factor;
        const float factor4 = factor2 * factor2;
        return INV_VOLUME * factor4 * (RADIUS + 4.0f * distance);
    }

    static inline float WDrv(float distance2) noexcept
    {
        const float distance = glm::sqrt(distance2);
        const float factor = RADIUS - distance;
        const float factor3 = factor * factor * factor;
        return -20.0f * INV_VOLUME * distance * factor3;
    }
};

