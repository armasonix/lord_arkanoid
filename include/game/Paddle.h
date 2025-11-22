#pragma once
#include <SFML/Graphics.hpp>
#include "input/InputSystem.h"
#include "game/GameObject.h"
#include "game/Collision.h"
#include "game/Collidable.h"

namespace ark
{

    class Paddle : public GameObject
    {
    public:
        explicit Paddle(const sf::Vector2f& size, const sf::Vector2f& startPos);
        void update(float dt, const InputSystem& input, const sf::RenderWindow& window);

        void update(float dt) override;
        void render(sf::RenderTarget& rt) const override;

        ObjKind kind() const override { return ObjKind::Paddle; }
        sf::Vector2f position() const override { return m_shape.getPosition(); }
        void setPosition(const sf::Vector2f& pos);
        void setSizeMultiplier(float factor);
        void setSpeedMultiplier(float factor);

        AABB aabb() const;
        const sf::RectangleShape& shape() const { return m_shape; }
        float speedMultiplier() const { return m_speedMultiplier; }
        float baseSpeed() const { return m_baseSpeed; }

        const Collidable& collider() const { return m_collider; }
        Collidable& collider() { return m_collider; }

    private:
        sf::RectangleShape m_shape;
        float m_baseSpeed = 900.f;
        float m_speedMultiplier = 1.f;
        sf::Vector2f m_baseSize;
        Collidable m_collider;
    };

} // namespace ark