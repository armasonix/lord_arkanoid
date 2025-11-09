#include "utils/DebugOverlay.h"
#include "utils/Profiler.h"
#include <sstream>

namespace ark 
{

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

    const sf::FloatRect b = m_text.getLocalBounds();
    const sf::Vector2f bgPos{ m_pos.x + b.left - m_pad, m_pos.y + b.top - m_pad };
    const sf::Vector2f bgSize{ b.width + 2.f * m_pad, b.height + 2.f * m_pad };

    m_back.setPosition(bgPos);
    m_back.setSize(bgSize);

    rt.draw(m_back);
    rt.draw(m_text);
}

} // namespace ark