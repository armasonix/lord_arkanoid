#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include "game/GameObject.h"
#include "game/Collision.h"
#include "game/Collidable.h"

namespace ark
{
    class Paddle; // fwd

    class Ball : public GameObject
    {
    public:
        Ball(float radius, const sf::Vector2f& startPos, const sf::Vector2f& startVel);

        void update(float dt, const sf::RenderWindow& window, const Paddle& paddle);
        void update(float dt) override;
        void render(sf::RenderTarget& rt) const override;

        ObjKind kind() const override { return ObjKind::Ball; }
        sf::Vector2f position() const override { return m_shape.getPosition(); }

        float        speed() const;
        Circle       circle() const;
        sf::Vector2f velocity() const { return m_vel; }
        float        speedScale() const { return m_speedScale; }
        bool         piercing() const { return m_piercing; }
        bool         randomBounce() const { return m_randomBounce; }
        const sf::Color& baseColor() const { return m_baseColor; }

        bool fellBelow() const { return m_fellBelow; }
        void reset(const sf::Vector2f& pos, const sf::Vector2f& vel);

        void setSpeedScale(float scale);
        void setPiercing(bool piercing);
        void setRandomBounce(bool randomBounce);
        void setColor(const sf::Color& color);

        const Collidable& collider() const { return m_collider; }
        Collidable& collider() { return m_collider; }

    private:
        sf::CircleShape m_shape;
        sf::Vector2f    m_vel;
        bool            m_fellBelow{ false };
        Collidable      m_collider;
        float           m_speedScale{ 1.f };
        float           m_baseTargetSpeed{ 560.f };
        bool            m_piercing{ false };
        bool            m_randomBounce{ false };
        sf::Color       m_baseColor;
        std::mt19937    m_rng{ std::random_device{}() };
    };

} // namespace ark