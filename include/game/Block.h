#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "game/GameObject.h"
#include "game/Collision.h"
#include "game/Collidable.h"
#include "gfx/ColorEffect.h"

namespace ark
{
    enum class BlockType { Low, Med, Hev };

    // abstract
    class BlockBase : public GameObject
    {
    public:
        virtual ~BlockBase() = default;
        void update(float dt) override { (void)dt; }
        ObjKind kind() const override { return ObjKind::Block; }

        virtual sf::Vector2f position() const override = 0;
        virtual void render(sf::RenderTarget& rt) const override = 0;
        virtual void onHit() = 0;
        virtual bool alive() const = 0;
        virtual AABB aabb() const = 0;
        virtual BlockType type() const = 0;
        virtual const Collidable& collider() const = 0;
        virtual Collidable& collider() = 0;
    };

    class Block final : public BlockBase
    {
    public:
        Block(BlockType t,
            int maxHp,
            const sf::Vector2f& pos,
            const sf::Vector2f& size,
            const sf::Color& base,
            const gfx::ColorEffect* effect = nullptr)
            : m_type(t)
            , m_hp(maxHp)
            , m_hpMax(maxHp)
            , m_base(base)
            , m_effect(effect)
            , m_collider(*this, AABB{ pos - size * 0.5f, size })
        {
            m_shape.setSize(size);
            m_shape.setOrigin(size.x * 0.5f, size.y * 0.5f);
            m_shape.setPosition(pos);
            m_shape.setFillColor(base);
        }

        sf::Vector2f position() const override
        {
            return m_shape.getPosition();
        }

        void render(sf::RenderTarget& rt) const override
        {
            if (!alive())
                return;

            if (m_effect && m_type != BlockType::Low)
            {
                float sat = static_cast<float>(m_hp) / static_cast<float>(m_hpMax);
                m_shape.setFillColor(m_effect->apply(m_base, sat));
            }
            else
            {
                m_shape.setFillColor(m_base);
            }

            rt.draw(m_shape);
        }

        void onHit() override
        {
            if (m_hp > 0)
                --m_hp;
        }

        bool alive() const override
        {
            return m_hp > 0;
        }

        AABB aabb() const override
        {
            auto s = m_shape.getSize();
            auto p = m_shape.getPosition() - s * 0.5f;
            return AABB{ p, s };
        }

        BlockType type() const override
        {
            return m_type;
        }

        const Collidable& collider() const override { return m_collider; }
        Collidable& collider() override { return m_collider; }

    private:
        BlockType m_type;
        int m_hp;
        int m_hpMax;
        mutable sf::RectangleShape m_shape;
        sf::Color m_base;
        const gfx::ColorEffect* m_effect;
        Collidable m_collider;
    };

    inline std::unique_ptr<BlockBase> makeBlock(BlockType t,
        const sf::Vector2f& pos,
        const sf::Vector2f& size,
        const gfx::ColorEffect* effect)
    {
        switch (t)
        {
        case BlockType::Low:
            return std::make_unique<Block>(t, 1, pos, size, sf::Color(120, 220, 255), effect);
        case BlockType::Med:
            return std::make_unique<Block>(t, 2, pos, size, sf::Color(255, 170, 60), effect);
        case BlockType::Hev:
            return std::make_unique<Block>(t, 3, pos, size, sf::Color(255, 80, 80), effect);
        }
        return nullptr;
    }

} // namespace ark