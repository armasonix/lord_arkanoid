#pragma once
#include "core/State.h"
#include <SFML/Graphics.hpp>

namespace ark
{

class GameOverState : public State 
{
public:
    GameOverState(Context ctx, int finalScore)
        : State(ctx), m_finalScore(finalScore)
    {}

    using State::State;
    void onEnter() override;
    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderTarget& rt) override;

private:
    int m_finalScore = 0;
    sf::Text m_title;
    float m_time{ 0.f };
    sf::Color m_titleBase{ 255, 120, 120 };
};

} // namespace ark