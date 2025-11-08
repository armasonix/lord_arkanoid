#pragma once
#include "core/State.h"
#include <memory>
#include "game/World.h"

namespace ark
{

class PlayState : public State 
{
public:
    using State::State;
    void onEnter() override;
    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderTarget& rt) override;

private:
    std::unique_ptr<World> m_world; // RAII
    float m_dtLast{ 0.f };
};

} // namespace ark