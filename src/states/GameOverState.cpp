#include "states/GameOverState.h"
#include "core/StateMachine.h"
#include "core/Resources.h"
#include "core/HighScoreTable.h"
#include "states/MenuState.h"
#include "audio/MusicService.h"

namespace ark
{

    void GameOverState::onEnter()
    {
        auto& font = m_ctx.resources->font("mono");

        m_title.setFont(font);
        m_title.setCharacterSize(32);
        m_title.setFillColor(sf::Color(255, 120, 120));

        std::string text =
            "GAME OVER\n"
            "Score: " + std::to_string(m_finalScore) + "\n"
            "Press Enter to main menu";

        m_title.setString(text);

        if (m_ctx.window)
        {
            auto sz = m_ctx.window->getSize();
            float cx = static_cast<float>(sz.x) * 0.5f;
            float cy = static_cast<float>(sz.y) * 0.5f;

            auto b = m_title.getLocalBounds();
            m_title.setOrigin(b.left + b.width * 0.5f,
                b.top + b.height * 0.5f);
            m_title.setPosition(cx, cy);
        }

        if (m_ctx.music)
            m_ctx.music->stop();
    }

    void GameOverState::handleEvent(const sf::Event& e)
    {
        if (e.type == sf::Event::KeyPressed &&
            e.key.code == sf::Keyboard::Enter)
        {
            m_ctx.states->clear();
            m_ctx.states->push<MenuState>();
        }
    }

    void GameOverState::render(sf::RenderTarget& rt)
    {
        rt.draw(m_title);
    }

} // namespace ark