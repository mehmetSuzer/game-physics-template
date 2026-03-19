
#include "particle_pool.hpp"

class IObstacle
{
protected:
    glm::vec3 m_centre;
    glm::vec3 m_scale;
    float m_damping;

    IObstacle() = default;

    IObstacle(const glm::vec3& centre, const glm::vec3& scale, float damping) noexcept
        : m_centre(centre), m_scale(scale), m_damping(damping) { }

public:
    virtual void Bound(ParticlePool& pool) const noexcept = 0;
    virtual void Draw(Renderer& renderer) const noexcept = 0;
};

class Container final : public IObstacle
{
private:
    static constexpr glm::vec3 COLOUR = glm::vec3(1.0f);

public:
    Container() = default;

    Container(const glm::vec3& centre, const glm::vec3& scale, float damping) noexcept
        : IObstacle(centre, scale, damping) { }

    void Bound(ParticlePool& pool) const noexcept override;
    void Draw(Renderer& renderer) const noexcept override;
};

class Wall final : public IObstacle
{
private:
    static constexpr glm::vec3 COLOUR = glm::vec3(1.0f);

public:
    Wall() = default;
    
    Wall(const glm::vec3& centre, const glm::vec3& scale, float damping) noexcept
        : IObstacle(centre, scale, damping) { }

    void Bound(ParticlePool& pool) const noexcept override;
    void Draw(Renderer& renderer) const noexcept override;
};

