#pragma once
#include "core/State.h"
#include "game/Game.h"
#include <memory>
#include <string>

namespace ark
{
class PlayState : public State
{
public:
    PlayState(Context ctx, bool loadSavedGame = false, std::string savePath = DEFAULT_SAVE_FILE)
        : State(ctx)
        , m_loadSavedGame(loadSavedGame)
        , m_initialSavePath(std::move(savePath))
    {}
    void onEnter() override;
    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderTarget& rt) override;

private:
    std::unique_ptr<Game> m_game;
    float m_dtLast{ 0.f };
    bool m_goQueued{ false };
    bool m_winQueued{ false };

    sf::Text m_scoreText;
    bool m_loadSavedGame{ false };
    std::string m_initialSavePath;
};

} // namespace ark