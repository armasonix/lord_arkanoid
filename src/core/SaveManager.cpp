#include "core/SaveManager.h"
#include <fstream>

namespace ark
{
    bool SaveManager::save(const GameSave& save, const std::string& filename)
    {
        std::ofstream out(filename);
        if (!out.is_open()) return false;

        out << save.score << " " << save.lives << "\n";
        out << save.ballPosition.x << " " << save.ballPosition.y << " "
            << save.ballVelocity.x << " " << save.ballVelocity.y << "\n";
        out << save.paddlePosition.x << " " << save.paddlePosition.y << "\n";

        out << save.blocks.size() << "\n";
        for (auto& b : save.blocks)
        {
            out << b.position.x << " " << b.position.y << " "
                << b.destroyed << "\n";
        }
        return true;
    }

    bool SaveManager::load(GameSave& save, const std::string& filename)
    {
        std::ifstream in(filename);
        if (!in.is_open()) return false;

        in >> save.score >> save.lives;
        in >> save.ballPosition.x >> save.ballPosition.y
            >> save.ballVelocity.x >> save.ballVelocity.y;
        in >> save.paddlePosition.x >> save.paddlePosition.y;

        std::size_t count = 0;
        in >> count;
        save.blocks.resize(count);

        for (std::size_t i = 0; i < count; ++i)
        {
            in >> save.blocks[i].position.x
                >> save.blocks[i].position.y
                >> save.blocks[i].destroyed;
        }
        return true;
    }

} // namespace ark