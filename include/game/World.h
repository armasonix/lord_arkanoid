#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"

namespace ark
{

class World 
{
public:
    explicit World(sf::RenderWindow& window);

    void update(float dt, class InputSystem& input);
    void render(sf::RenderTarget& rt);

    float ballSpeed() const { return m_ball.speed(); }
    bool isBallLost() const { return m_ball.fellBelow(); }
    void resetBall();

private:
    sf::RenderWindow& m_window;
    Paddle m_paddle;
    Ball   m_ball;
    sf::RectangleShape m_boundsTop, m_boundsLeft, m_boundsRight;
};

} // namespace ark