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

    m_blocks.generateResponsive(
        window,
        /*desiredRows*/ 6,
        /*targetBlockW*/ 72.f,
        /*targetBlockH*/ 28.f,
        /*gap*/ 8.f,
        /*topMargin*/ 60.f,
        /*sideMargin*/ 12.f,
        /*bottomSafety*/ 120.f);
}

void World::update(float dt, class InputSystem& input) 
{
    PROFILE_SCOPE("World::update");
    m_paddle.update(dt, input, m_window);
    m_ball.update(dt, m_window, m_paddle);

    handleBallBlocksCollision();

    // shake
    m_shake.update(dt);
}

void World::render(sf::RenderTarget& rt)
{
    PROFILE_SCOPE("World::render");

    // care of current view
    sf::View prevView;
    if (auto* wnd = dynamic_cast<sf::RenderWindow*>(&rt))
    {
        prevView = wnd->getView();
        sf::View view = prevView; // copy current
        auto off = m_shake.offset();
        view.setCenter(prevView.getCenter() + off);
        wnd->setView(view);
    }

    rt.draw(m_boundsTop);
    rt.draw(m_boundsLeft);
    rt.draw(m_boundsRight);

    m_blocks.render(rt);
    m_paddle.render(rt);
    m_ball.render(rt);

    if (auto* wnd = dynamic_cast<sf::RenderWindow*>(&rt)) 
    {
        wnd->setView(prevView);
    }
}

void World::resetBall() 
{
    m_ball.reset({ m_window.getSize().x * 0.5f, m_window.getSize().y * 0.6f }, { 200.f, -300.f });
}

void World::handleBallBlocksCollision() 
{
    auto cir = m_ball.circle();

    for (auto& b : m_blocks.blocks()) 
    {
        if (!b->alive()) continue;
        sf::Vector2f n; float pen;
        if (circleIntersectsAabb(cir, b->aabb(), n, pen)) 
        {
            auto pos = m_ball.position();
            pos += n * pen;
            // reflect
            auto vel = m_ball.velocity();
            vel = vel - 2.f * (vel.x * n.x + vel.y * n.y) * n;

            m_ball.reset(pos, vel);

            // damage
            int hpBefore = 1;
            if (b->alive()) { b->onHit(); }

            bool destroyedNow = !b->alive();
            // shake: low -1 hp, strong = destroy
            if (destroyedNow)
            {
                m_shake.trigger(/*mag*/4.0f, /*dur*/0.10f);
            }
            else {
                m_shake.trigger(/*mag*/1.8f, /*dur*/0.06f);
            }
            break;
        }
    }
}

} // namespace ark