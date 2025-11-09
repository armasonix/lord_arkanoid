#pragma once
#include "game/ICollidable.h"
namespace ark 
{
    enum class ShapeKind { Box, Disk };
    class Collidable final : public ICollidable 
    {
    public:
        Collidable(const GameObject& owner, AABB box) : m_owner(owner), m_shape(ShapeKind::Box), m_box(box) {}
        Collidable(const GameObject& owner, Circle cir) : m_owner(owner), m_shape(ShapeKind::Disk), m_cir(cir) {}
        AABB aabb() const override 
        {
            return (m_shape == ShapeKind::Box) ? m_box : AABB{ {m_cir.c.x - m_cir.r, m_cir.c.y - m_cir.r}, {2 * m_cir.r, 2 * m_cir.r} };
        }
        bool test(const ICollidable& other, CollisionManifold& out) const override;
        ObjKind ownerKind() const override { return m_owner.kind(); }
        void setBox(AABB b) { m_shape = ShapeKind::Box; m_box = b; }
        void setCircle(Circle c) { m_shape = ShapeKind::Disk; m_cir = c; }
    private:
        const GameObject& m_owner;
        ShapeKind m_shape;
        AABB   m_box{};
        Circle m_cir{};
    };
}