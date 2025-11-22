#pragma once
#include <memory>
#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "core/State.h"
#include "core/ScoreSystem.h"
#include "core/GameEventBus.h"
#include "game/World.h"
#include "game/GameState.h"

namespace ark
{
    constexpr const char* DEFAULT_SAVE_FILE = "savegame.dat";

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
        int lives() const;
        void resetLevel();
        void togglePause();
        bool isPaused() const;
        bool saveToFile(const std::string& path);
        bool loadFromFile(const std::string& path);

        World& world();
        const World& world() const;
        World* worldPtr();
        const World* worldPtr() const;
        Context& context();
        const Context& context() const;

        ScoreSystem& score() { return m_scoreSystem; }
        GameEventBus& events() { return m_eventBus; }

    private:
        void changeState(std::unique_ptr<GameState> state);
        Context m_ctx;
        std::unique_ptr<World>     m_world;
        std::unique_ptr<GameState> m_state;
        ScoreSystem m_scoreSystem;
        GameEventBus m_eventBus;
    };

} // namespace ark