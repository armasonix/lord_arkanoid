#pragma once
#include <SFML/Graphics.hpp>
namespace ark 
{
    struct IModule 
    {
        virtual ~IModule() = default;
        virtual void handleEvent(const sf::Event&) = 0;
        virtual void update(float dt) = 0;
        virtual void render(sf::RenderTarget&) = 0;
    };
}