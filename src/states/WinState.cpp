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
        m_title.setPosition(80.f, 100.f);

        m_prompt.setFont(font);
        m_prompt.setString("Wanna Play again?  [Y]es / [N]o");
        m_prompt.setCharacterSize(24);
        m_prompt.setFillColor(sf::Color(220, 220, 240));
        m_prompt.setPosition(80.f, 170.f);
        if (m_ctx.music) m_ctx.music->stop();

        if (m_ctx.sfx) m_ctx.sfx->playEnsure(Sfx::Win);
    }

    void WinState::handleEvent(const sf::Event& e) 
    {
        if (e.type != sf::Event::KeyPressed) return;
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