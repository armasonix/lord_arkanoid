#pragma once
#include "core/State.h"
#include <string>

namespace ark
{

class BootState : public State 
{
public:
    using State::State;
    void onEnter() override {/* noop */ }
    void handleEvent(const sf::Event&) override {}
    void update(float) override;
    void render(sf::RenderTarget&) override {}

private:
    bool m_done{ false };
};

} // namespace ark