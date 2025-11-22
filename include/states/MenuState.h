#pragma once
#include "core/State.h"
#include <SFML/Graphics.hpp>
#include "gfx/TwinkleStarfield.h"
#include <vector>
#include <string>


namespace ark
{

class MenuState : public State 
{
public:
    using State::State;
    void onEnter() override;
    void handleEvent(const sf::Event& e) override;
    void update(float) override;
    void render(sf::RenderTarget& rt) override;

protected:
    sf::Text m_title, m_hint;
    gfx::TwinkleStarfield m_starfield;
    std::vector<std::string> m_items;
    std::vector<sf::Text>    m_textItems;
    int m_selected = 0;
    float m_time{ 0.f };
    sf::Color m_hintBaseColor{ 220, 180, 180 };
};

} // namespace ark