#pragma once
#include <vector>
#include <string>

namespace ark
{

    class HighScoreTable
    {
    public:
        void load(const std::string& filename);
        void save(const std::string& filename) const;
        void submitScore(int score);
        const std::vector<int>& scores() const { return m_scores; }

    private:
        static constexpr std::size_t MaxEntries = 10;
        std::vector<int> m_scores;
    };

} // namespace ark