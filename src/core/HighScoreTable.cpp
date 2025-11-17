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

        int value = 0;
        while (in >> value)
        {
            m_scores.push_back(value);
        }

        std::sort(m_scores.begin(), m_scores.end(), std::greater<int>());

        if (m_scores.size() > MaxEntries)
            m_scores.resize(MaxEntries);
    }

    void HighScoreTable::save(const std::string& filename) const
    {
        std::ofstream out(filename, std::ios::trunc);
        if (!out.is_open())
            return;

        for (int s : m_scores)
        {
            out << s << "\n";
        }
    }

    void HighScoreTable::submitScore(int score)
    {
        m_scores.push_back(score);
        std::sort(m_scores.begin(), m_scores.end(), std::greater<int>());

        if (m_scores.size() > MaxEntries)
            m_scores.resize(MaxEntries);
    }

} // namespace ark