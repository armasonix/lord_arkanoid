#include "game/BlockGrid.h"
#include <cmath>

namespace ark 
{

    void BlockGrid::generate(const sf::RenderWindow& win, int rows, float bw, float bh, float gap, float topMargin) 
    {
        m_blocks.clear();

        float W = (float)win.getSize().x;
        int cols = (int)std::floor((W + gap) / (bw + gap));
        cols = std::max(cols, 1);

        float gridW = cols * (bw + gap) - gap;
        float startX = (W - gridW) * 0.5f;

        std::uniform_int_distribution<int> dist(0, 2); // 0=Low,1=Med,2=Hev

        for (int r = 0; r < rows; ++r) 
        {
            float y = topMargin + r * (bh + gap) + bh * 0.5f;
            for (int c = 0; c < cols; ++c) 
            {
                float x = startX + c * (bw + gap) + bw * 0.5f;
                BlockType t = static_cast<BlockType>(dist(m_rng));
                m_blocks.emplace_back(makeBlock(t, { x, y }, { bw, bh }, &m_desat));
            }
        }
    }

    void BlockGrid::generateResponsive(const sf::RenderWindow& win,
        int desiredRows,
        float targetBW, float targetBH,
        float gap,
        float topMargin,
        float sideMargin,
        float bottomSafety)
    {
        m_blocks.clear();

        const float W = (float)win.getSize().x;
        const float H = (float)win.getSize().y;

        const float side = gap;

        const float availW = std::max(0.f, W - 2.f * side);
        int cols = (int)std::floor((availW + gap) / (targetBW + gap));
        cols = std::max(cols, 1);

        const float minBW = 40.f;

        auto computeBW = [&](int c) -> float 
            {
                return (W - (c + 1) * gap) / (float)c; 
            };

        float bw = computeBW(cols);
        while (bw < minBW && cols > 1)
        {
            --cols;
            bw = computeBW(cols);
        }

        const float availH = std::max(0.f, H - bottomSafety - topMargin);
        int maxRowsFit = (int)std::floor((availH + gap) / (targetBH + gap));
        int rows = std::clamp(desiredRows, 1, std::max(1, maxRowsFit));

        const float bh = targetBH;
        const float startX = gap;

        std::uniform_int_distribution<int> dist(0, 2); // 0=Low,1=Med,2=Hev

        for (int r = 0; r < rows; ++r) 
        {
            const float y = topMargin + r * (bh + gap) + bh * 0.5f;
            for (int c = 0; c < cols; ++c) 
            {
                const float x = startX + c * (bw + gap) + bw * 0.5f;
                BlockType t = static_cast<BlockType>(dist(m_rng));
                m_blocks.emplace_back(makeBlock(t, { x, y }, { bw, bh }, &m_desat));
            }
        }
    }

    void BlockGrid::render(sf::RenderTarget& rt) 
    {
        for (auto& b : m_blocks) if (b->alive()) b->render(rt);
    }

    bool BlockGrid::allDestroyed() const 
    {
        for (auto& b : m_blocks) if (b->alive()) return false;
        return true;
    }

} // namespace ark