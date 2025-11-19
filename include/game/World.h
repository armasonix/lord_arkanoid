#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "BlockGrid.h"
#include "gfx/Starfield.h"
#include "vfx/CameraShake.h"
#include "core/GameEventBus.h"
#include "core/GameSave.h"
#include "core/SaveManager.h"

namespace ark
{
    class World
    {
    public:
        explicit World(sf::RenderWindow& window, GameEventBus& events);

        void update(float dt, class InputSystem& input);
        void render(sf::RenderTarget& rt);

        float ballSpeed() const { return m_ball.speed(); }

        bool isBallLost() const;
        void resetBall();
        bool victory() const { return m_blocks.allDestroyed(); }

        // memento / save system
        GameSave createSave() const;
        void     applySave(const GameSave& save);
        bool     saveToFile(const std::string& path);
        bool     loadFromFile(const std::string& path);

    private:
        void handleBallBlocksCollision();
        void updateLifeSystem();

        gfx::Starfield    m_starfield;
        sf::RenderWindow& m_window;
        GameEventBus& m_events;

        Paddle     m_paddle;
        Ball       m_ball;
        BlockGrid  m_blocks;
        CameraShake m_shake;

        sf::RectangleShape m_boundsTop, m_boundsLeft, m_boundsRight;
        sf::View           m_baseView;

        GameSave m_lifeSave{};
        bool     m_hasLifeSave{ false };

        int  m_lives = 3;
        int  m_score = 0;
        bool m_gameOver = false;
    };

} // namespace ark