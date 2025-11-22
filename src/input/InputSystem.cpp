#include "input/InputSystem.h"

namespace ark
{

    void InputSystem::newFrame()
    {
        m_pausePressed = false;
        m_debugTogglePressed = false;
    }

    void InputSystem::handleEvent(const sf::Event& e, const sf::RenderWindow& window)
    {
        switch (e.type)
        {
        case sf::Event::KeyPressed:
            switch (e.key.code)
            {
            case sf::Keyboard::Left:
                m_left = true;
                m_mode = ControlMode::Keyboard;
                break;
            case sf::Keyboard::Right:
                m_right = true;
                m_mode = ControlMode::Keyboard;
                break;
            case sf::Keyboard::Escape:
                m_pausePressed = true;
                break;
            case sf::Keyboard::F3:
                m_debugTogglePressed = true;
                break;
            default:
                break;
            }
            break;

        case sf::Event::KeyReleased:
            if (e.key.code == sf::Keyboard::Left)
                m_left = false;
            if (e.key.code == sf::Keyboard::Right)
                m_right = false;
            break;

        case sf::Event::MouseMoved:
        {
            auto pos = sf::Mouse::getPosition(window);
            m_mouseX = static_cast<float>(pos.x);
            m_mode = ControlMode::Mouse;
        }
        break;

        default:
            break;
        }
    }

} // namespace ark