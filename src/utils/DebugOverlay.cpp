#include "utils/DebugOverlay.h"
#include "utils/Profiler.h"
#include <sstream>

void DebugOverlay::draw(sf::RenderTarget& rt, float fps, float dt, float ballSpeed) 
{
    if (!m_visible) return;
    std::ostringstream ss;
    ss.setf(std::ios::fixed); ss.precision(2);
    ss << "FPS: " << fps << "\n";
    ss << "dt (ms): " << dt * 1000.0f << "\n";
    ss << "Ball speed: " << ballSpeed << "\n";
    ss << "Profiler (last):\n";
    int c = 0;
    for (auto& s : Profiler::instance().samples())
    {
        ss << "  " << s.name << ": " << s.ms << " ms\n";
        if (++c >= 8) break;
    }
    m_text.setString(ss.str());
    m_text.setPosition(8.f, 8.f);
    rt.draw(m_text);
}