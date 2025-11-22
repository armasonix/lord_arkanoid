#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "game/GameObject.h"
#include "game/Collision.h"

namespace ark
{
    class World;
    class Paddle;

    enum class BonusKind
    {
        FireBall,
        FragileBlocks,
        RandomBounce,
        SmallPaddle,
        SlowPaddle,
    };

    // strategy for apply and revert powerup effects.
    class BonusEffect
    {
    public:
        virtual ~BonusEffect() = default;
        virtual BonusKind kind() const = 0;
        virtual float duration() const = 0;
        virtual void apply(World& world) = 0;
        virtual void revert(World& world) = 0;

        // reapply hook to reset timers or refresh visuals.
        virtual void refresh(World& world) { apply(world); }
    };

    class BonusPickup : public GameObject
    {
    public:
        BonusPickup(BonusKind kind,
            std::unique_ptr<BonusEffect> effect,
            const sf::Texture& texture,
            const sf::Vector2f& pos);

        void update(float dt) override;
        void update(float dt, const sf::RenderWindow& window);
        void render(sf::RenderTarget& rt) const override;

        ObjKind kind() const override { return ObjKind::Bonus; }
        sf::Vector2f position() const override { return m_sprite.getPosition(); }

        bool alive() const { return m_alive; }
        bool tryCollect(const Paddle& paddle);
        std::unique_ptr<BonusEffect> takeEffect();
        BonusKind bonusKind() const { return m_kind; }

    private:
        BonusKind m_kind;
        sf::Sprite m_sprite;
        sf::Texture m_texture;
        sf::Vector2f m_velocity;
        float m_gravity{ 900.f };
        bool m_alive{ true };
        float m_radius{ 16.f };
        float m_spawnTimer{ 0.f };
        std::unique_ptr<BonusEffect> m_effect;
    };

    std::unique_ptr<BonusPickup> makeBonusPickup(BonusKind kind, const sf::Vector2f& pos);

} // namespace ark