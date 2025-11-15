#pragma once
#include <memory>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "core/State.h"
#include "game/World.h"
#include "game/GameState.h"

namespace ark
{
    // Game incapsulate World
    // update / render + isBallLost, victory, ballSpeed
    class Game
    {
    public:
        explicit Game(Context ctx);
        void handleEvent(const sf::Event& e);
        void update(float dt);
        void render(sf::RenderTarget& rt);
        bool isBallLost() const;
        bool victory() const;
        float ballSpeed() const;
        void resetLevel();

        World& world();
        const World& world() const;
        World* worldPtr();
        const World* worldPtr() const;
        Context& context();
        const Context& context() const;

    private:
        void changeState(std::unique_ptr<GameState> state);
        Context m_ctx;
        std::unique_ptr<World>     m_world;
        std::unique_ptr<GameState> m_state;
    };

} // namespace ark