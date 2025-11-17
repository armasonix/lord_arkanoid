#pragma once
#include "core/State.h"
#include <SFML/Graphics.hpp>

namespace ark 
{

    class WinState : public State 
    {
    public:
        WinState(Context ctx, int finalScore)
            : State(ctx), m_finalScore(finalScore)
        {}

        using State::State;
        void onEnter() override;
        void handleEvent(const sf::Event& e) override;
        void update(float) override {}
        void render(sf::RenderTarget& rt) override;

    private:
        int m_finalScore = 0;
        sf::Text m_title;
        sf::Text m_prompt;
    };

} // namespace ark