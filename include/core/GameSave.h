#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace ark
{

    struct BlockSave
    {
        sf::Vector2f position;
        bool destroyed = false;
    };

    struct GameSave
    {
        sf::Vector2f ballPosition;
        sf::Vector2f ballVelocity;

        sf::Vector2f paddlePosition;
        std::vector<BlockSave> blocks;

        int score = 0;
        int lives = 3;
    };

} // namespace ark