#pragma once
#include <SFML/System.hpp>

class FPSCounter 
{
public:
    void frameStart() { /* noop */ }
    void frameEnd() 
    {
        ++m_frames;
        if (m_clock.getElapsedTime().asSeconds() >= 0.5f) 
        {
            m_fps = static_cast<float>(m_frames) / m_clock.restart().asSeconds();
            m_frames = 0;
        }
    }
    float fps() const { return m_fps; }
private:
    sf::Clock m_clock;
    int m_frames{ 0 };
    float m_fps{ 0.f };
};