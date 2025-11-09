#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
namespace ark 
{
    class Game; // fwd
    class GameState 
    {
    public:
        explicit GameState(Game& g) : m_game(g) {}
        virtual ~GameState() = default;
        virtual void onEnter() {}
        virtual void onExit() {}
        virtual void handleEvent(const sf::Event&) = 0;
        virtual void update(float dt) = 0;
        virtual void render(sf::RenderTarget&) = 0;
    protected:
        Game& m_game;
    };
}