#pragma once
#include "core/GameEvents.h"

namespace ark
{

    class ScoreSystem : public IGameEventListener
    {
    public:
        ScoreSystem() = default;

        void reset();
        void setScore(int score);
        int score() const { return m_score; }

        // event react
        void onGameEvent(const GameEvent& event) override;

    private:
        int m_score = 0;
    };

} // namespace ark