#pragma once
#include <string>

namespace ark
{

    // event type
    enum class GameEventType
    {
        BlockDestroyed
    };

    // blocks type
    enum class BlockKind
    {
        Normal,
        Hard,
        Bonus,
        Unknown
    };

    struct GameEvent
    {
        GameEventType type{ GameEventType::BlockDestroyed };
        BlockKind blockKind = BlockKind::Unknown;
    };

    // observer interface
    class IGameEventListener
    {
    public:
        virtual ~IGameEventListener() = default;
        virtual void onGameEvent(const GameEvent& event) = 0;
    };

} // namespace ark