#pragma once
#include "game/GameObject.h"
#include "game/Collision.h"

namespace ark
{
    struct ICollidable
    {
        virtual ~ICollidable() = default;
        virtual AABB aabb() const = 0;
        virtual bool test(const ICollidable& other, CollisionManifold& out) const = 0;
        virtual ObjKind ownerKind() const = 0;
    };

} // namespace ark