#pragma once
#include "core/State.h"
#include <SFML/Graphics.hpp>
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
    std::vector<std::string> m_items;
    std::vector<sf::Text>    m_textItems;
    int m_selected = 0;
};

} // namespace ark