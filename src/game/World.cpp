#include "game/World.h"
#include "utils/Profiler.h"

namespace ark
{

World::World(sf::RenderWindow& window)
    : m_window(window)
    , m_paddle({ 160.f, 20.f }, { window.getSize().x * 0.5f, window.getSize().y - 50.f })
    , m_ball(10.f, { window.getSize().x * 0.5f, window.getSize().y * 0.6f }, { 200.f, -300.f })
{
    // bounds
    m_boundsTop.setSize({ (float)window.getSize().x, 6.f });
    m_boundsTop.setFillColor(sf::Color(80, 80, 100));
    m_boundsTop.setPosition(0.f, 0.f);

    m_boundsLeft.setSize({ 6.f, (float)window.getSize().y });
    m_boundsLeft.setFillColor(sf::Color(80, 80, 100));
    m_boundsLeft.setPosition(0.f, 0.f);

    m_boundsRight = m_boundsLeft;
    m_boundsRight.setPosition((float)window.getSize().x - 6.f, 0.f);
}

void World::update(float dt, class InputSystem& input) 
{
    PROFILE_SCOPE("World::update");
    m_paddle.update(dt, input, m_window);
    m_ball.update(dt, m_window, m_paddle);
}

void World::render(sf::RenderTarget& rt) 
{
    PROFILE_SCOPE("World::render");
    rt.draw(m_boundsTop);
    rt.draw(m_boundsLeft);
    rt.draw(m_boundsRight);
    m_paddle.render(rt);
    m_ball.render(rt);
}

void World::resetBall() 
{
    m_ball.reset({ m_window.getSize().x * 0.5f, m_window.getSize().y * 0.6f }, { 200.f, -300.f });
}

} // namespace ark