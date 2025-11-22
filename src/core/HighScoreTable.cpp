#include "core/HighScoreTable.h"
#include <fstream>
#include <algorithm>

namespace ark
{

    void HighScoreTable::load(const std::string& filename)
    {
        m_scores.clear();
        std::ifstream in(filename);
        if (!in.is_open())
            return;

        std::string name;
        int score = 0;

        while (in >> name >> score)
        {
            m_scores.push_back({ name, score });
        }

        std::sort(m_scores.begin(), m_scores.end(),
            [](const auto& a, const auto& b) { return a.score > b.score; });

        if (m_scores.size() > MaxEntries)
            m_scores.resize(MaxEntries);
    }

    void HighScoreTable::save(const std::string& filename) const
    {
        std::ofstream out(filename, std::ios::trunc);
        if (!out.is_open())
            return;

        for (auto& e : m_scores)
            out << e.name << " " << e.score << "\n";
    }

    void HighScoreTable::submitScore(const std::string& name, int score)
    {
        m_scores.push_back({ name, score });

        std::sort(m_scores.begin(), m_scores.end(),
            [](const auto& a, const auto& b) { return a.score > b.score; });

        if (m_scores.size() > MaxEntries)
            m_scores.resize(MaxEntries);
    }

} // namespace ark