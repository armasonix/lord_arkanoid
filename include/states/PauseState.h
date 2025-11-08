#pragma once
#include "core/State.h"
#include <SFML/Graphics.hpp>

namespace ark
{

class PauseState : public State 
{
public:
    using State::State;
    void onEnter() override;
    void handleEvent(const sf::Event& e) override;
    void update(float) override;
    void render(sf::RenderTarget& rt) override;

private:
    sf::Text m_text;
};

} // namespace ark