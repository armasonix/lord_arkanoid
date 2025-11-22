#include "game/Paddle.h"
#include "game/Collision.h"
#include <algorithm>

namespace ark
{

    Paddle::Paddle(const sf::Vector2f& size, const sf::Vector2f& startPos)
        : m_collider(*this, AABB{ startPos - size * 0.5f, size })
    {
        m_shape.setSize(size);
        m_shape.setOrigin(size.x * 0.5f, size.y * 0.5f);
        m_shape.setFillColor(sf::Color(220, 220, 240));
        m_shape.setPosition(startPos);
        m_baseSize = size;
    }

    void Paddle::update(float dt)
    {
        (void)dt;
    }

    void Paddle::update(float dt, const InputSystem& input, const sf::RenderWindow& window)
    {
        auto pos = m_shape.getPosition();

        if (input.mode() == ControlMode::Keyboard)
        {
            float dir = (input.right() ? 1.f : 0.f) - (input.left() ? 1.f : 0.f);
            pos.x += dir * (m_baseSpeed * m_speedMultiplier) * dt;
        }
        else
        {
            pos.x = input.mouseX();
        }

        float halfW = m_shape.getSize().x * 0.5f;
        float winW = static_cast<float>(window.getSize().x);

        if (pos.x < halfW)
            pos.x = halfW;
        if (pos.x > winW - halfW)
            pos.x = winW - halfW;

        m_shape.setPosition(pos);

        auto s = m_shape.getSize();
        auto p = m_shape.getPosition() - s * 0.5f;
        m_collider.setBox(AABB{ p, s });
    }

    void Paddle::setPosition(const sf::Vector2f& pos)
    {
        m_shape.setPosition(pos);

        const auto size = m_shape.getSize();
        m_collider.setBox(AABB{ pos - size * 0.5f, size });
    }

    void Paddle::setSizeMultiplier(float factor)
    {
        const float clamped = std::clamp(factor, 0.35f, 2.5f);
        sf::Vector2f newSize = m_baseSize * clamped;
        auto pos = m_shape.getPosition();
        m_shape.setSize(newSize);
        m_shape.setOrigin(newSize.x * 0.5f, newSize.y * 0.5f);
        m_shape.setPosition(pos);
        m_collider.setBox(AABB{ pos - newSize * 0.5f, newSize });
    }

    void Paddle::setSpeedMultiplier(float factor)
    {
        m_speedMultiplier = std::clamp(factor, 0.25f, 2.5f);
    }

    void Paddle::render(sf::RenderTarget& rt) const
    {
        rt.draw(m_shape);
    }

    AABB Paddle::aabb() const
    {
        auto s = m_shape.getSize();
        auto p = m_shape.getPosition() - s * 0.5f;
        return AABB{ p, s };
    }

} // namespace ark