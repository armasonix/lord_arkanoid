#include "game/Ball.h"
#include <cmath>

Ball::Ball(float r, const sf::Vector2f& startPos, const sf::Vector2f& startVel)
    : m_vel(startVel)
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
}

float Ball::speed() const 
{
    return std::sqrt(m_vel.x * m_vel.x + m_vel.y * m_vel.y);
}

Circle Ball::circle() const 
{
    return { m_shape.getPosition(), m_shape.getRadius() };
}

void Ball::update(float dt, const sf::RenderWindow& window, const Paddle& paddle) 
{
    auto pos = m_shape.getPosition();
    pos += m_vel * dt;

    const float w = static_cast<float>(window.getSize().x);
    const float h = static_cast<float>(window.getSize().y);
    float r = m_shape.getRadius();

    // walls X
    if (pos.x - r < 0.f) { pos.x = r; m_vel.x = -m_vel.x; }
    if (pos.x + r > w) { pos.x = w - r; m_vel.x = -m_vel.x; }
    // top
    if (pos.y - r < 0.f) { pos.y = r; m_vel.y = -m_vel.y; }
    // bot
    if (pos.y - r > h) { m_fellBelow = true; }

    // paddle collision
    sf::Vector2f n; float pen;
    if (circleIntersectsAabb({ pos, r }, paddle.aabb(), n, pen)) 
    {
        pos += n * pen;
        // add the angle from the point of impact along X
        float hitOffset = (pos.x - (paddle.aabb().pos.x + paddle.aabb().size.x * 0.5f)) / (paddle.aabb().size.x * 0.5f);
        m_vel = m_vel - 2.f * (m_vel.x * n.x + m_vel.y * n.y) * n; // reflect
        m_vel.x += hitOffset * 200.f;
        // stabilize
        float target = 560.f;
        float s = speed();
        if (s > 1.f) m_vel *= (target / s);
    }

    m_shape.setPosition(pos);
}

void Ball::render(sf::RenderTarget& rt) const 
{
    rt.draw(m_shape);
}