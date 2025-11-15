#include "game/Ball.h"
#include "game/Paddle.h"
#include "game/Collision.h"

#include <cmath>

namespace ark
{

    Ball::Ball(float r, const sf::Vector2f& startPos, const sf::Vector2f& startVel)
        : m_vel(startVel)
        , m_collider(*this, Circle{ startPos, r })
    {
        m_shape.setRadius(r);
        m_shape.setOrigin(r, r);
        m_shape.setFillColor(sf::Color(240, 240, 255));
        m_shape.setPosition(startPos);
    }

    void Ball::reset(const sf::Vector2f& pos, const sf::Vector2f& vel)
    {
        m_shape.setPosition(pos);
        m_vel = vel;
        m_fellBelow = false;

        const float r = m_shape.getRadius();
        m_collider.setCircle(Circle{ pos, r });
    }

    float Ball::speed() const
    {
        return std::sqrt(m_vel.x * m_vel.x + m_vel.y * m_vel.y);
    }

    Circle Ball::circle() const
    {
        return Circle{ m_shape.getPosition(), m_shape.getRadius() };
    }

    void Ball::update(float dt)
    {
        (void)dt;
    }

    void Ball::update(float dt, const sf::RenderWindow& window, const Paddle& paddle)
    {
        auto pos = m_shape.getPosition();
        pos += m_vel * dt;

        const float w = static_cast<float>(window.getSize().x);
        const float h = static_cast<float>(window.getSize().y);
        const float r = m_shape.getRadius();

        // wall X
        if (pos.x - r < 0.f)
        {
            pos.x = r;
            m_vel.x = -m_vel.x;
        }
        if (pos.x + r > w)
        {
            pos.x = w - r;
            m_vel.x = -m_vel.x;
        }

        // top
        if (pos.y - r < 0.f)
        {
            pos.y = r;
            m_vel.y = -m_vel.y;
        }

        // bot
        if (pos.y - r > h)
        {
            m_fellBelow = true;
        }

        // collider update
        m_collider.setCircle(Circle{ pos, r });

        // collide with paddle

        CollisionManifold man{};
        if (m_collider.test(paddle.collider(), man))
        {
            pos += man.normal * man.penetration;

            // reflect speed from normal
            float dot = m_vel.x * man.normal.x + m_vel.y * man.normal.y;
            m_vel = m_vel - 2.f * dot * man.normal;

            // spin x
            const AABB padBox = paddle.aabb();
            const float padCenter = padBox.pos.x + padBox.size.x * 0.5f;
            const float hitOffset =
                (pos.x - padCenter) / (padBox.size.x * 0.5f);

            m_vel.x += hitOffset * 200.f;

            // stabilize speed
            const float targetSpeed = 560.f;
            const float s = speed();
            if (s > 1.f)
            {
                m_vel *= (targetSpeed / s);
            }
        }

        // write position
        m_shape.setPosition(pos);

        // collider sync
        m_collider.setCircle(Circle{ pos, r });
    }

    void Ball::render(sf::RenderTarget& rt) const
    {
        rt.draw(m_shape);
    }

} // namespace ark