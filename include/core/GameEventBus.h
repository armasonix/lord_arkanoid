#pragma once
#include <vector>
#include "core/GameEvents.h"

namespace ark
{

    class GameEventBus
    {
    public:
        void addListener(IGameEventListener* listener);
        void removeListener(IGameEventListener* listener);

        // dispatcher sent for all subscribers
        void dispatch(const GameEvent& event);

    private:
        std::vector<IGameEventListener*> m_listeners;
    };

} // namespace ark