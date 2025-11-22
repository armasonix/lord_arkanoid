#include "core/ScoreSystem.h"

namespace ark
{

    void ScoreSystem::reset()
    {
        m_score = 0;
    }

    void ScoreSystem::setScore(int score)
    {
        m_score = score;
    }

    void ScoreSystem::onGameEvent(const GameEvent& event)
    {
        if (event.type != GameEventType::BlockDestroyed)
            return;

        switch (event.blockKind)
        {
        case BlockKind::Normal:
            m_score += 50;
            break;
        case BlockKind::Hard:
            m_score += 150;
            break;
        case BlockKind::Bonus:
            m_score += 100;
            break;
        default:
            m_score += 10;
            break;
        }
    }

} // namespace ark