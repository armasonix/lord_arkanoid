#include "states/PlayState.h"
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
        // create Game
        m_game = std::make_unique<Game>(m_ctx);
        if (m_ctx.music)
            m_ctx.music->playLevel(true);

        m_goQueued = false;
        m_winQueued = false;
    }

    void PlayState::handleEvent(const sf::Event& e)
    {
        if (m_game)
            m_game->handleEvent(e);
    }

    void PlayState::update(float dt)
    {
        m_dtLast = dt;

        if (m_game)
            m_game->update(dt);

        // lose
        if (!m_goQueued && m_game && m_game->isBallLost())
        {
            if (m_ctx.music) m_ctx.music->stop();
            if (m_ctx.sfx)   m_ctx.sfx->playEnsure(Sfx::Lose);

            m_ctx.states->push<GameOverState>();
            m_goQueued = true;
            return;
        }

        // win
        if (!m_winQueued && m_game && m_game->victory())
        {
            if (m_ctx.music) m_ctx.music->stop();
            if (m_ctx.sfx)   m_ctx.sfx->playEnsure(Sfx::Win);

            m_ctx.states->push<WinState>();
            m_winQueued = true;
            return;
        }

        // debug overlay
        if (m_ctx.input && m_ctx.input->debugTogglePressed())
        {
            m_ctx.debug->setVisible(!m_ctx.debug->visible());
        }
    }

    void PlayState::render(sf::RenderTarget& rt)
    {
        if (m_game)
            m_game->render(rt);

        if (m_ctx.debug->visible())
        {
            const float speed = m_game ? m_game->ballSpeed() : 0.f;
            m_ctx.debug->draw(rt, m_ctx.debug->fps(), m_dtLast, speed);
        }
    }

} // namespace ark