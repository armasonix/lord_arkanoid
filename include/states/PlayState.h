#pragma once
#include "core/State.h"
#include <memory>
#include "game/Game.h"

namespace ark
{
    class PlayState : public State
    {
    public:
        using State::State;
        void onEnter() override;
        void handleEvent(const sf::Event& e) override;
        void update(float dt) override;
        void render(sf::RenderTarget& rt) override;

    private:
        std::unique_ptr<Game> m_game;  // RAII game
        float m_dtLast{ 0.f };
        bool m_goQueued{ false };
        bool m_winQueued{ false };
    };

} // namespace ark