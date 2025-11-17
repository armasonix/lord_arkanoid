#include "core/GameEventBus.h"

namespace ark
{

    void GameEventBus::addListener(IGameEventListener* listener)
    {
        m_listeners.push_back(listener);
    }

    void GameEventBus::removeListener(IGameEventListener* listener)
    {
        m_listeners.erase(
            std::remove(m_listeners.begin(), m_listeners.end(), listener),
            m_listeners.end());
    }

    void GameEventBus::dispatch(const GameEvent& event)
    {
        for (auto* l : m_listeners)
            l->onGameEvent(event);
    }

} // namespace ark