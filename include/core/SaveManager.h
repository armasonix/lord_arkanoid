#pragma once
#include "core/GameSave.h"
#include <string>

namespace ark
{
    class SaveManager
    {
    public:
        static bool save(const GameSave& save, const std::string& filename);
        static bool load(GameSave& save, const std::string& filename);
    };

} // namespace ark