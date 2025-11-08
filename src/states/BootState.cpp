#include "states/BootState.h"
#include "states/MenuState.h"
#include "core/StateMachine.h"

namespace ark 
{

    void BootState::update(float) 
    {
        if (m_done) return;
        m_ctx.states->pop();
        m_ctx.states->push<MenuState>();
        m_done = true;
    }

} // namespace ark