#pragma once
#include <SFML/Graphics.hpp>
#include "game/Collision.h"
#include "game/Paddle.h"

namespace ark
{

class Ball 
{
public:
    Ball(float radius, const sf::Vector2f& startPos, const sf::Vector2f& startVel);

    void update(float dt, const sf::RenderWindow& window, const Paddle& paddle);
    void render(sf::RenderTarget& rt) const;

    float speed() const;
    Circle circle() const;
    sf::Vector2f velocity() const { return m_vel; }
    sf::Vector2f position() const { return m_shape.getPosition(); }

    bool fellBelow() const { return m_fellBelow; }
    void reset(const sf::Vector2f& pos, const sf::Vector2f& vel);

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_vel;
    bool m_fellBelow{ false };
};

} // namespace ark