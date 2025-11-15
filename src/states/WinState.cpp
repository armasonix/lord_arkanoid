#include "states/WinState.h"
#include "core/Resources.h"
#include "core/StateMachine.h"
#include "states/PlayState.h"
#include "states/MenuState.h"
#include "audio/SoundService.h"
#include "audio/MusicService.h"

namespace ark
{
    void WinState::onEnter()
    {
        auto& font = m_ctx.resources->font("mono");
        m_title.setFont(font);
        m_title.setString("YOU WIN!");
        m_title.setCharacterSize(48);
        m_title.setFillColor(sf::Color(120, 255, 120));
        m_prompt.setFont(font);
        m_prompt.setString("Wanna Play again?  [Y]es / [N]o");
        m_prompt.setCharacterSize(24);
        m_prompt.setFillColor(sf::Color(220, 220, 240));

        if (m_ctx.window)
        {
            auto sz = m_ctx.window->getSize();
            float cx = static_cast<float>(sz.x) * 0.5f;
            float cy = static_cast<float>(sz.y) * 0.5f;

            // header
            {
                auto b = m_title.getLocalBounds();
                m_title.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_title.setPosition(cx, cy - 40.f);
            }

            // hint
            {
                auto b = m_prompt.getLocalBounds();
                m_prompt.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_prompt.setPosition(cx, cy + 20.f);
            }
        }

        if (m_ctx.music)
            m_ctx.music->stop();
        if (m_ctx.sfx)
            m_ctx.sfx->playEnsure(Sfx::Win);
    }

    void WinState::handleEvent(const sf::Event& e)
    {
        if (e.type != sf::Event::KeyPressed)
            return;

        if (e.key.code == sf::Keyboard::Y)
        {
            m_ctx.states->clear();
            m_ctx.states->push<PlayState>();
        }
        else if (e.key.code == sf::Keyboard::N)
        {
            m_ctx.states->clear();
            m_ctx.states->push<MenuState>();
        }
    }

    void WinState::render(sf::RenderTarget& rt)
    {
        rt.draw(m_title);
        rt.draw(m_prompt);
    }

} // namespace ark