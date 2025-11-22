#pragma once
#include "core/State.h"
#include <SFML/Graphics.hpp>

namespace ark
{

    class EnterNameState : public State
    {
    public:
        EnterNameState(Context ctx, int finalScore, bool win)
            : State(ctx), m_score(finalScore), m_win(win)
        {}

        void onEnter() override;
        void handleEvent(const sf::Event& e) override;
        void update(float dt) override;
        void render(sf::RenderTarget& rt) override;

    private:
        int  m_score;
        bool m_win;

        std::string m_name = "ABC";
        sf::Text m_title;
        sf::Text m_nameText;
        sf::Text m_prompt;
        float m_time{ 0.f };
        sf::Color m_promptBase{ 220, 220, 220 };
    };

}