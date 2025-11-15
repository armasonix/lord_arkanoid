#include "states/MenuState.h"
#include "core/StateMachine.h"
#include "core/Resources.h"
#include "states/PlayState.h"
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
        m_hint.setString("Press Enter to Play");
        m_hint.setCharacterSize(24);
        m_hint.setFillColor(sf::Color(200, 200, 220));

        if (m_ctx.window)
        {
            auto sz = m_ctx.window->getSize();
            float cx = static_cast<float>(sz.x) * 0.5f;
            float cy = static_cast<float>(sz.y) * 0.5f;
            {
                auto b = m_title.getLocalBounds();
                m_title.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_title.setPosition(cx, cy - 40.f);
            }

            {
                auto b = m_hint.getLocalBounds();
                m_hint.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_hint.setPosition(cx, cy + 20.f);
            }
        }

        if (m_ctx.music)
            m_ctx.music->playTheme(true);
    }

    void MenuState::handleEvent(const sf::Event& e)
    {
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
        {
            m_ctx.states->push<PlayState>();
        }
    }

    void MenuState::update(float) {}

    void MenuState::render(sf::RenderTarget& rt)
    {
        rt.draw(m_title);
        rt.draw(m_hint);
    }

} // namespace ark