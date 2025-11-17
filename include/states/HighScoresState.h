#pragma once
#include "states/MenuState.h"
#include "core/HighScoreTable.h"
#include <SFML/Graphics/Text.hpp>

namespace ark
{

    class HighScoresState : public MenuState
    {
    public:
        using MenuState::MenuState; // reuse constructor

        void onEnter() override;
        void handleEvent(const sf::Event& e) override;
        void render(sf::RenderTarget& rt) override;

    private:
        HighScoreTable m_table;
        sf::Text m_title;
        sf::Text m_scores;
        sf::Text m_prompt;
    };

} // namespace ark