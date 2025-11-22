#include "states/MenuState.h"
#include "core/StateMachine.h"
#include "core/Resources.h"
#include "core/SaveManager.h"
#include "states/PlayState.h"
#include "states/HighScoresState.h"
#include "audio/MusicService.h"

namespace ark
{

    void MenuState::onEnter()
    {
        auto& font = m_ctx.resources->font("mono");
        m_title.setFont(font);
        m_title.setString("LORD ARKANOID");
        m_title.setCharacterSize(48);
        m_title.setFillColor(sf::Color::White);

        m_hint.setFont(font);
        m_hint.setString("");
        m_hint.setCharacterSize(18);
        m_hint.setFillColor(sf::Color(220, 180, 180));

        m_items = { "PLAY", "LOAD GAME", "HIGHSCORES", "EXIT" };

        m_textItems.clear();
        for (auto& s : m_items)
        {
            sf::Text t;
            t.setFont(font);
            t.setString(s);
            t.setCharacterSize(32);
            t.setFillColor(sf::Color(200, 200, 220));
            m_textItems.push_back(t);
        }

        if (m_ctx.window)
        {
            auto sz = m_ctx.window->getSize();
            float cx = static_cast<float>(sz.x) * 0.5f;
            float cy = static_cast<float>(sz.y) * 0.5f;
            {
                auto b = m_title.getLocalBounds();
                m_title.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_title.setPosition(cx, cy - 180.f);
            }

            float startY = cy - 40.f;
            float step = 50.f;

            for (std::size_t i = 0; i < m_textItems.size(); ++i)
            {
                auto b = m_textItems[i].getLocalBounds();
                m_textItems[i].setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_textItems[i].setPosition(cx, startY + step * i);
            }

            {
                auto b = m_hint.getLocalBounds();
                m_hint.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_hint.setPosition(cx, cy + step * static_cast<float>(m_textItems.size()) + 10.f);
            }
        }

        m_selected = 0;

        if (m_ctx.music)
            m_ctx.music->playTheme(true);
    }

    void MenuState::handleEvent(const sf::Event& e)
    {
        if (e.type != sf::Event::KeyPressed)
            return;

        if (e.key.code == sf::Keyboard::Up)
        {
            m_selected--;
            if (m_selected < 0)
                m_selected = static_cast<int>(m_items.size()) - 1;
        }
        else if (e.key.code == sf::Keyboard::Down)
        {
            m_selected++;
            if (m_selected >= static_cast<int>(m_items.size()))
                m_selected = 0;
        }
        else if (e.key.code == sf::Keyboard::Enter)
        {
            const std::string& choice = m_items[m_selected];

            if (choice == "PLAY")
            {
                m_ctx.states->push<PlayState>();
            }
            else if (choice == "LOAD GAME")
            {
                GameSave save;
                if (SaveManager::load(save, DEFAULT_SAVE_FILE))
                {
                    m_ctx.states->push<PlayState>(true, DEFAULT_SAVE_FILE);
                }
                else
                {
                    m_hint.setString("No save file found");

                    auto b = m_hint.getLocalBounds();
                    if (m_ctx.window)
                    {
                        auto sz = m_ctx.window->getSize();
                        float cx = static_cast<float>(sz.x) * 0.5f;
                        float cy = static_cast<float>(sz.y) * 0.5f;
                        float startY = cy - 40.f;
                        float step = 50.f;
                        m_hint.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                        m_hint.setPosition(cx, startY + step * static_cast<float>(m_textItems.size()) + 10.f);
                    }
                }
            }
            else if (choice == "HIGHSCORES")
            {
                m_ctx.states->push<HighScoresState>();
            }
            else if (choice == "EXIT")
            {
                if (m_ctx.window)
                    m_ctx.window->close();
            }
        }
    }

    void MenuState::update(float) {}

    void MenuState::render(sf::RenderTarget& rt)
    {
        rt.draw(m_title);

        // hover
        for (std::size_t i = 0; i < m_textItems.size(); ++i)
        {
            if ((int)i == m_selected)
                m_textItems[i].setFillColor(sf::Color(255, 255, 180));
            else
                m_textItems[i].setFillColor(sf::Color(200, 200, 220));

            rt.draw(m_textItems[i]);
        }

        if (!m_hint.getString().isEmpty())
            rt.draw(m_hint);
    }

} // namespace ark