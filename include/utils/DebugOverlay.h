#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class DebugOverlay 
{
public:
    explicit DebugOverlay(sf::Font* font) : m_font(font) 
    {
        m_text.setFont(*m_font);
        m_text.setCharacterSize(14);
        m_text.setFillColor(sf::Color::Green);
    }
    void setVisible(bool v) { m_visible = v; }
    bool visible() const { return m_visible; }

    void draw(sf::RenderTarget& rt, float fps, float dt, float ballSpeed);

private:
    bool m_visible{ false };
    sf::Font* m_font{};
    sf::Text m_text;
};