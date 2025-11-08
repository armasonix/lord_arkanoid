#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "BlockGrid.h"
#include "vfx/CameraShake.h"

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

    bool victory() const { return m_blocks.allDestroyed(); }
private:
    void handleBallBlocksCollision();

    sf::RenderWindow& m_window;
    Paddle m_paddle;
    Ball   m_ball;

    BlockGrid m_blocks;
    CameraShake m_shake;

    sf::RectangleShape m_boundsTop, m_boundsLeft, m_boundsRight;
    sf::View m_baseView;
};

} // namespace ark