#pragma once
#include <SFML/Graphics.hpp>
namespace ark 
{
    enum class ObjKind { Paddle, Ball, Block, Unknown };
    struct CollisionManifold { sf::Vector2f normal{}; float penetration{ 0.f }; };

    class GameObject 
    {
    public:
        virtual ~GameObject() = default;
        virtual void handleEvent(const sf::Event&) {}
        virtual void update(float dt) = 0;
        virtual void render(sf::RenderTarget&) = 0;
        virtual ObjKind kind() const = 0;
        virtual sf::Vector2f position() const = 0;
        virtual void onCollision(const CollisionManifold&) {}
    };
}