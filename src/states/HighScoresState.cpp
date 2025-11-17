#include "states/HighScoresState.h"
#include "core/Resources.h"
#include "core/StateMachine.h"
#include "states/MenuState.h"

namespace ark
{

    void HighScoresState::onEnter()
    {
        auto& font = m_ctx.resources->font("mono");

        // header
        m_title.setFont(font);
        m_title.setCharacterSize(48);
        m_title.setFillColor(sf::Color(240, 240, 240));
        m_title.setString("HIGH SCORES");

        m_table.load("highscores.txt");

        // top 10
        std::string text;
        const auto& scores = m_table.scores();
        for (std::size_t i = 0; i < scores.size(); ++i)
        {
            text += std::to_string(i + 1) + ". " + std::to_string(scores[i]) + "\n";
        }
        if (scores.empty())
            text = "No scores yet.";

        m_scores.setFont(font);
        m_scores.setCharacterSize(30);
        m_scores.setFillColor(sf::Color(220, 220, 240));
        m_scores.setString(text);

        // hint
        m_prompt.setFont(font);
        m_prompt.setCharacterSize(22);
        m_prompt.setFillColor(sf::Color(200, 200, 200));
        m_prompt.setString("Press ENTER to return in main menu");

        if (m_ctx.window)
        {
            auto sz = m_ctx.window->getSize();
            float cx = sz.x * 0.5f;
            float cy = sz.y * 0.5f;

            auto b1 = m_title.getLocalBounds();
            m_title.setOrigin(b1.left + b1.width * 0.5f, b1.top + b1.height * 0.5f);
            m_title.setPosition(cx, cy - 200.f);

            auto b2 = m_scores.getLocalBounds();
            m_scores.setOrigin(b2.left + b2.width * 0.5f, b2.top + b2.height * 0.5f);
            m_scores.setPosition(cx, cy - 20.f);

            auto b3 = m_prompt.getLocalBounds();
            m_prompt.setOrigin(b3.left + b3.width * 0.5f, b3.top + b3.height * 0.5f);
            m_prompt.setPosition(cx, cy + 220.f);
        }
    }

    void HighScoresState::handleEvent(const sf::Event& e)
    {
        if (e.type == sf::Event::KeyPressed &&
            e.key.code == sf::Keyboard::Enter)
        {
            m_ctx.states->pop();
        }
    }

    void HighScoresState::render(sf::RenderTarget& rt)
    {
        rt.draw(m_title);
        rt.draw(m_scores);
        rt.draw(m_prompt);
    }

} // namespace ark