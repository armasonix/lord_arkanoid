#pragma once
#include "game/ICollidable.h"
#include "game/Collision.h"

namespace ark
{
    enum class ShapeKind { Box, Disk };

    class Collidable final : public ICollidable
    {
    public:
        Collidable(const GameObject& owner, AABB box);
        Collidable(const GameObject& owner, Circle cir);

        AABB aabb() const override;
        bool test(const ICollidable& other, CollisionManifold& out) const override;
        ObjKind ownerKind() const override;

        void setBox(AABB b);
        void setCircle(Circle c);

    private:
        const GameObject& m_owner;
        ShapeKind m_shape;
        AABB   m_box{};
        Circle m_cir{};
    };

} // namespace ark