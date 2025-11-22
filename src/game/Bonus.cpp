#include "game/Bonus.h"
#include "game/World.h"
#include "game/Paddle.h"
#include "game/Ball.h"

#include <SFML/Graphics.hpp>
#include <map>

namespace ark
{
    namespace
    {
        sf::Texture makeBallTexture(const sf::Color& core, const sf::Color& glow)
        {
            sf::RenderTexture rt;
            rt.create(48, 48);
            rt.clear(sf::Color::Transparent);

            sf::CircleShape halo(20.f);
            halo.setOrigin(20.f, 20.f);
            halo.setPosition(24.f, 24.f);
            sf::Color glowColor = glow;
            glowColor.a = 130;
            halo.setFillColor(glowColor);
            rt.draw(halo);

            sf::CircleShape ball(14.f);
            ball.setOrigin(14.f, 14.f);
            ball.setPosition(24.f, 24.f);
            ball.setFillColor(core);
            ball.setOutlineColor(sf::Color(240, 240, 255, 180));
            ball.setOutlineThickness(3.f);
            rt.draw(ball);

            rt.display();

            sf::Texture tex;
            tex.loadFromImage(rt.getTexture().copyToImage());
            tex.setSmooth(true);
            return tex;
        }

        const sf::Texture& textureFor(BonusKind kind)
        {
            static std::map<BonusKind, sf::Texture> cache;
            auto it = cache.find(kind);
            if (it != cache.end())
                return it->second;

            sf::Color core;
            sf::Color glow;
            switch (kind)
            {
            case BonusKind::FireBall:
                core = sf::Color(255, 170, 80);
                glow = sf::Color(255, 120, 40);
                break;
            case BonusKind::FragileBlocks:
                core = sf::Color(160, 220, 255);
                glow = sf::Color(110, 200, 255);
                break;
            case BonusKind::RandomBounce:
                core = sf::Color(210, 120, 255);
                glow = sf::Color(170, 80, 240);
                break;
            case BonusKind::SmallPaddle:
                core = sf::Color(255, 120, 140);
                glow = sf::Color(230, 70, 120);
                break;
            case BonusKind::SlowPaddle:
                core = sf::Color(140, 200, 120);
                glow = sf::Color(90, 170, 90);
                break;
            default:
                core = sf::Color::White;
                glow = sf::Color(200, 200, 200);
                break;
            }

            auto res = cache.emplace(kind, makeBallTexture(core, glow));
            return res.first->second;
        }

        class FireBallEffect final : public BonusEffect
        {
        public:
            BonusKind kind() const override { return BonusKind::FireBall; }
            float duration() const override { return 8.f; }
            void apply(World& world) override { world.enableFireBall(); }
            void revert(World& world) override { world.disableFireBall(); }
        };

        class FragileBlocksEffect final : public BonusEffect
        {
        public:
            BonusKind kind() const override { return BonusKind::FragileBlocks; }
            float duration() const override { return 6.f; }
            void apply(World& world) override { world.enableFragileBlocks(); }
            void revert(World& world) override { world.disableFragileBlocks(); }
        };

        class RandomBounceEffect final : public BonusEffect
        {
        public:
            BonusKind kind() const override { return BonusKind::RandomBounce; }
            float duration() const override { return 6.5f; }
            void apply(World& world) override { world.enableRandomBounce(); }
            void revert(World& world) override { world.disableRandomBounce(); }
        };

        class SmallPaddleEffect final : public BonusEffect
        {
        public:
            BonusKind kind() const override { return BonusKind::SmallPaddle; }
            float duration() const override { return 7.f; }
            void apply(World& world) override { world.enableSmallPaddle(); }
            void revert(World& world) override { world.disableSmallPaddle(); }
        };

        class SlowPaddleEffect final : public BonusEffect
        {
        public:
            BonusKind kind() const override { return BonusKind::SlowPaddle; }
            float duration() const override { return 7.f; }
            void apply(World& world) override { world.enableSlowPaddle(); }
            void revert(World& world) override { world.disableSlowPaddle(); }
        };

        std::unique_ptr<BonusEffect> makeEffect(BonusKind kind)
        {
            switch (kind)
            {
            case BonusKind::FireBall: return std::make_unique<FireBallEffect>();
            case BonusKind::FragileBlocks: return std::make_unique<FragileBlocksEffect>();
            case BonusKind::RandomBounce: return std::make_unique<RandomBounceEffect>();
            case BonusKind::SmallPaddle: return std::make_unique<SmallPaddleEffect>();
            case BonusKind::SlowPaddle: return std::make_unique<SlowPaddleEffect>();
            }
            return nullptr;
        }
    } // namespace

    BonusPickup::BonusPickup(BonusKind kind,
        std::unique_ptr<BonusEffect> effect,
        const sf::Texture& texture,
        const sf::Vector2f& pos)
        : m_kind(kind)
        , m_texture(texture)
        , m_effect(std::move(effect))
    {
        m_sprite.setTexture(m_texture);
        m_sprite.setOrigin(m_texture.getSize().x * 0.5f, m_texture.getSize().y * 0.5f);
        m_sprite.setPosition(pos);
        m_velocity = { 0.f, -120.f };
        m_radius = m_texture.getSize().x * 0.5f;
    }

    void BonusPickup::update(float dt)
    {
        (void)dt;
    }

    void BonusPickup::update(float dt, const sf::RenderWindow& window)
    {
        if (!m_alive)
            return;

        m_spawnTimer += dt;
        m_velocity.y += m_gravity * dt;

        auto pos = m_sprite.getPosition();
        pos += m_velocity * dt;
        m_sprite.setPosition(pos);

        if (pos.y - m_radius > static_cast<float>(window.getSize().y))
        {
            m_alive = false;
        }
    }

    void BonusPickup::render(sf::RenderTarget& rt) const
    {
        if (!m_alive)
            return;
        rt.draw(m_sprite);
    }

    bool BonusPickup::tryCollect(const Paddle& paddle)
    {
        if (!m_alive)
            return false;

        sf::Vector2f normal{};
        float        penetration{};
        if (circleIntersectsAabb(Circle{ m_sprite.getPosition(), m_radius }, paddle.aabb(), normal, penetration))
        {
            m_alive = false;
            return true;
        }
        return false;
    }

    std::unique_ptr<BonusEffect> BonusPickup::takeEffect()
    {
        return std::move(m_effect);
    }

    std::unique_ptr<BonusPickup> makeBonusPickup(BonusKind kind, const sf::Vector2f& pos)
    {
        auto effect = makeEffect(kind);
        const auto& tex = textureFor(kind);
        return std::make_unique<BonusPickup>(kind, std::move(effect), tex, pos);
    }

} // namespace ark