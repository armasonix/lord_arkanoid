#pragma once
#include <vector>
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>
#include "game/Block.h"

namespace ark 
{

    class BlockGrid 
    {
    public:
        BlockGrid() = default;

        // grid generation
        void generate(const sf::RenderWindow& win, int rows, float blockW, float blockH, float gap, float topMargin);

        // render
        void render(sf::RenderTarget& rt);
        std::vector<std::unique_ptr<BlockBase>>& blocks() { return m_blocks; }
        const std::vector<std::unique_ptr<BlockBase>>& blocks() const { return m_blocks; }

        void generateResponsive(const sf::RenderWindow& win,
            int desiredRows,
            float targetBlockW, float targetBlockH,
            float gap,
            float topMargin,
            float sideMargin,
            float bottomSafety = 120.f);

        bool allDestroyed() const;

    private:
        std::vector<std::unique_ptr<BlockBase>> m_blocks;
        gfx::DesaturateEffect m_desat; // default
        std::mt19937 m_rng{ std::random_device{}() };
    };

} // namespace ark