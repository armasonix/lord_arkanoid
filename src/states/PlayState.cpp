#include "states/PlayState.h"
#include "states/PauseState.h"
#include "states/WinState.h"
#include "states/GameOverState.h"
#include "core/StateMachine.h"
#include "utils/DebugOverlay.h"
#include "input/InputSystem.h"
#include "audio/SoundService.h"
#include "audio/MusicService.h"

namespace ark
{

void PlayState::onEnter() 
{
    m_world = std::make_unique<World>(*m_ctx.window); // RAII world ownership
    if (m_ctx.music) m_ctx.music->playLevel(true);
}

void PlayState::handleEvent(const sf::Event& e) 
{
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) 
    {
        m_ctx.states->push<PauseState>();
    }
}

void PlayState::update(float dt) 
{
    m_dtLast = dt;
    m_world->update(dt, *m_ctx.input);

    // lose
    if (!m_goQueued && m_world->isBallLost()) 
    {
        if (m_ctx.music) m_ctx.music->stop();
        if (m_ctx.sfx)   m_ctx.sfx->playEnsure(Sfx::Lose);
        m_ctx.states->push<GameOverState>();
        m_goQueued = true;
        return;
    }

    // win
    if (!m_winQueued && m_world->victory()) 
    {
        if (m_ctx.music) m_ctx.music->stop();
        if (m_ctx.sfx)   m_ctx.sfx->playEnsure(Sfx::Win);
        m_ctx.states->push<WinState>();
        m_winQueued = true;
        return;
    }

    if (m_ctx.input->debugTogglePressed()) 
    {
        m_ctx.debug->setVisible(!m_ctx.debug->visible());
    }
}

void PlayState::render(sf::RenderTarget& rt) 
{
    m_world->render(rt);
    if (m_ctx.debug->visible()) 
    {
        m_ctx.debug->draw(rt, m_ctx.debug->fps(), m_dtLast, m_world->ballSpeed());
    }
}

} // namespace ark