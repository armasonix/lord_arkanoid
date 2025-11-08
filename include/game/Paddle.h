#pragma once
#include <SFML/Graphics.hpp>
#include "input/InputSystem.h"

class Paddle 
{
public:
    explicit Paddle(const sf::Vector2f& size, const sf::Vector2f& startPos);

    void update(float dt, const InputSystem& input, const sf::RenderWindow& window);
    void render(sf::RenderTarget& rt) const;

    AABB aabb() const;
    const sf::RectangleShape& shape() const { return m_shape; }

private:
    sf::RectangleShape m_shape;
    float m_speed = 900.f;
};