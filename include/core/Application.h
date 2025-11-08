#pragma once
#include <SFML/Graphics.hpp>
#include "core/StateMachine.h"
#include "input/InputSystem.h"
#include "core/Resources.h"
#include "utils/FPSCounter.h"
#include "utils/DebugOverlay.h"

namespace ark 
{

class Application 
{
public:
    Application(unsigned w, unsigned h, const char* title);
    ~Application();
    void run();

private:
    sf::RenderWindow m_window;
    ResourceManager  m_resources;
    InputSystem      m_input;
    FPSCounter       m_fps;
    DebugOverlay     m_debug;
    StateMachine     m_states;

    float m_lastFps{ 0.f };
};

} // namespace ark