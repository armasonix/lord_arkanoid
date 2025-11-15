#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace ark 
{

class DebugOverlay 
{
public:
    explicit DebugOverlay(sf::Font* font) : m_font(font) 
    {
        m_text.setFont(*m_font);
        m_text.setCharacterSize(14);
        m_text.setFillColor(sf::Color::Green);

        m_back.setFillColor(sf::Color(0, 0, 0, 140));
        m_back.setOutlineThickness(0.f);
    }
    void setVisible(bool v) { m_visible = v; }
    bool visible() const { return m_visible; }

    void setFps(float v) { m_fpsVal = v; }
    float fps() const { return m_fpsVal; }

    void draw(sf::RenderTarget& rt, float fps, float dt, float ballSpeed);

private:
    bool m_visible{ false };
    sf::Font* m_font{};
    sf::Text m_text;

    sf::RectangleShape m_back;
    float m_pad{ 8.f };
    sf::Vector2f m_pos{ 8.f, 8.f };

    float m_fpsVal{ 0.f };
};

} // namespace ark