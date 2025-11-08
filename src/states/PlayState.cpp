#include "states/PlayState.h"
#include "states/PauseState.h"
#include "states/GameOverState.h"
#include "core/StateMachine.h"
#include "utils/DebugOverlay.h"
#include "input/InputSystem.h"

namespace ark
{

void PlayState::onEnter() 
{
    m_world = std::make_unique<World>(*m_ctx.window); // RAII world ownership
}

void PlayState::handleEvent(const sf::Event& e) 
{
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) 
    {
        m_ctx.states->push<PauseState>();
        m_ctx.states->apply();
    }
}

void PlayState::update(float dt) 
{
    m_dtLast = dt;
    m_world->update(dt, *m_ctx.input);

    // service hotkey
    if (m_ctx.input->debugTogglePressed()) 
    {
        m_ctx.debug->setVisible(!m_ctx.debug->visible());
    }

    if (m_world->isBallLost()) 
    {
        m_ctx.states->push<GameOverState>();
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