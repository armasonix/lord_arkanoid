#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"
#include "BlockGrid.h"
#include "gfx/Starfield.h"
#include "vfx/CameraShake.h"
#include "core/GameEventBus.h"
#include "core/GameSave.h"
#include "core/SaveManager.h"
#include "core/Resources.h"
#include <random>
#include <optional>
#include <functional>

namespace ark
{
    class ScoreSystem;

    class World
    {
    public:
        explicit World(sf::RenderWindow& window, ResourceManager& resources, GameEventBus& events, ScoreSystem& scoreSystem);

        void update(float dt, class InputSystem& input);
        void render(sf::RenderTarget& rt);

        float ballSpeed() const { return m_ball.speed(); }
        int   lives() const { return m_lives; }

        bool isBallLost() const;
        void resetBall();
        bool victory() const { return m_blocks.allDestroyed(); }

        // modifiers toggled by bonuses
        void enableFireBall();
        void disableFireBall();
        void enableFragileBlocks();
        void disableFragileBlocks();
        void enableRandomBounce();
        void disableRandomBounce();
        void enableSmallPaddle();
        void disableSmallPaddle();
        void enableSlowPaddle();
        void disableSlowPaddle();
        Ball& ball() { return m_ball; }
        Paddle& paddle() { return m_paddle; }

        // memento / save system
        GameSave createSave() const;
        void     applySave(const GameSave& save);
        bool     saveToFile(const std::string& path);
        bool     loadFromFile(const std::string& path);

    private:
        void handleBallBlocksCollision();
        void updateBonuses(float dt);
        void updateLifeSystem();
        void spawnBonus(const sf::Vector2f& pos);
        void activateBonus(std::unique_ptr<BonusEffect> effect);
        void resetBonusEffects();
        void updateBonusBanner(float dt);
        void showBonusBanner(BonusKind kind);

        gfx::Starfield    m_starfield;
        ResourceManager& m_resources;
        sf::RenderWindow& m_window;
        GameEventBus& m_events;
        ScoreSystem& m_scoreSystem;

        Paddle     m_paddle;
        Ball       m_ball;
        BlockGrid  m_blocks;
        CameraShake m_shake;
        std::vector<std::unique_ptr<BonusPickup>> m_bonusPickups;
        struct ActiveBonus
        {
            std::unique_ptr<BonusEffect> effect;
            float timeLeft;
        };
        std::vector<ActiveBonus> m_activeBonuses;

        sf::RectangleShape m_boundsTop, m_boundsLeft, m_boundsRight;
        sf::View           m_baseView;

        GameSave m_lifeSave{};
        bool     m_hasLifeSave{ false };

        int  m_lives = 2;
        bool m_gameOver = false;
        bool m_fireBallActive{ false };
        bool m_fragileBlocks{ false };
        bool m_randomBounceActive{ false };
        bool m_smallPaddleActive{ false };
        bool m_slowPaddleActive{ false };
        float m_fireballSpeedBoost{ 1.35f };
        std::mt19937 m_rng{ std::random_device{}() };
        struct BonusBanner
        {
            sf::Text text;
            float remaining{ 0.f };
            float duration{ 1.6f };
            std::function<sf::Color(float)> colorize;
        };
        std::optional<BonusBanner> m_bonusBanner;
    };

} // namespace ark