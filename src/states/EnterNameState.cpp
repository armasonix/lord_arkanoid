#include "states/EnterNameState.h"
#include "core/HighScoreTable.h"
#include "core/StateMachine.h"
#include "core/Resources.h"
#include "states/WinState.h"
#include "audio/SoundService.h"
#include "audio/MusicService.h"
#include "states/GameOverState.h"
#include <cctype>

namespace ark
{
    void EnterNameState::onEnter()
    {
        auto& font = m_ctx.resources->font("mono");

        HighScoreTable table;
        table.load("highscores.txt");
        table.submitScore(m_name, m_score);
        table.save("highscores.txt");

        m_title.setFont(font);
        m_title.setCharacterSize(40);
        m_title.setFillColor(sf::Color::White);
        m_title.setString("ENTER YOUR NAME");

        m_nameText.setFont(font);
        m_nameText.setCharacterSize(48);
        m_nameText.setFillColor(sf::Color(200, 255, 200));

        m_prompt.setFont(font);
        m_prompt.setCharacterSize(24);
        m_prompt.setFillColor(sf::Color(220, 220, 220));
        m_prompt.setString("Press ENTER to confirm");

        if (m_win)
        {
            if (m_ctx.music)
                m_ctx.music->stop();
            if (m_ctx.sfx)
                m_ctx.sfx->playEnsure(Sfx::Win);
        }

        if (m_ctx.window)
        {
            auto s = m_ctx.window->getSize();
            float cx = s.x * 0.5f;
            float cy = s.y * 0.5f;

            auto tb = m_title.getLocalBounds();
            m_title.setOrigin(tb.left + tb.width / 2, tb.top + tb.height / 2);
            m_title.setPosition(cx, cy - 150);

            m_nameText.setPosition(cx, cy - 20);

            auto pb = m_prompt.getLocalBounds();
            m_prompt.setOrigin(pb.left + pb.width / 2, pb.top + pb.height / 2);
            m_prompt.setPosition(cx, cy + 120);
        }
    }

    void EnterNameState::handleEvent(const sf::Event& e)
    {
        if (e.type == sf::Event::TextEntered)
        {
            if (e.text.unicode < 128u)
            {
                char c = static_cast<char>(e.text.unicode);

                if (std::isalpha(static_cast<unsigned char>(c)) && m_name.size() < 8)
                {
                    m_name += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                }
                else if (c == '\b' && !m_name.empty())
                {
                    m_name.pop_back();
                }

                if (m_name.empty())
                    m_name = "";
            }
        }
        else if (e.type == sf::Event::KeyPressed &&
            e.key.code == sf::Keyboard::Enter)
        {
            if (m_name.empty())
                m_name = "ABC";

            // save
            HighScoreTable table;
            table.load("highscores.txt");
            table.submitScore(m_name, m_score);
            table.save("highscores.txt");

            if (m_win)
                m_ctx.states->push<WinState>(m_score);
            else
                m_ctx.states->push<GameOverState>(m_score);
        }
    }

    void EnterNameState::render(sf::RenderTarget& rt)
    {
        rt.draw(m_title);

        m_nameText.setString(m_name);
        auto nb = m_nameText.getLocalBounds();
        m_nameText.setOrigin(nb.left + nb.width / 2, nb.top + nb.height / 2);
        rt.draw(m_nameText);

        rt.draw(m_prompt);
    }

}