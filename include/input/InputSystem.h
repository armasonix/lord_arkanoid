#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace ark
{
enum class ControlMode { Keyboard, Mouse };

class InputSystem
{
public:
    void newFrame();

    void handleEvent(const sf::Event& e, const sf::RenderWindow& window);
    bool left()  const { return m_left; }
    bool right() const { return m_right; }
    bool pausePressed() const { return m_pausePressed; }
    bool debugTogglePressed() const { return m_debugTogglePressed; }

    ControlMode mode() const { return m_mode; }
    float mouseX() const { return m_mouseX; }

private:
    bool m_left{ false };
    bool m_right{ false };
    bool m_pausePressed{ false };
    bool m_debugTogglePressed{ false };
    ControlMode m_mode{ ControlMode::Keyboard };
    float m_mouseX{ 0.f };
};

} // namespace ark