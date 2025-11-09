#pragma once
#include "game/GameObject.h"
namespace ark 
{
    struct AABB { sf::Vector2f pos, size; };
    struct Circle { sf::Vector2f c; float r; };
    struct ICollidable 
    {
        virtual ~ICollidable() = default;
        virtual AABB aabb() const = 0;
        virtual bool test(const ICollidable& other, CollisionManifold& out) const = 0;
        virtual ObjKind ownerKind() const = 0;
    };
}