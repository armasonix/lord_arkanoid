#pragma once
#include "core/HighScoreEntry.h"
#include <vector>
#include <string>

namespace ark
{

    class HighScoreTable
    {
    public:
        void load(const std::string& filename);
        void save(const std::string& filename) const;
        void submitScore(const std::string& name, int score);
        const std::vector<HighScoreEntry>& entries() const { return m_scores; }

    private:
        static constexpr std::size_t MaxEntries = 10;
        std::vector<HighScoreEntry> m_scores;
    };

} // namespace ark