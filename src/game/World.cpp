#include "game/World.h"
#include "utils/Profiler.h"

namespace ark
{

    World::World(sf::RenderWindow& window)
        : m_window(window)
        , m_paddle({ 160.f, 20.f }, { window.getSize().x * 0.5f, window.getSize().y - 50.f })
        , m_ball(10.f, { window.getSize().x * 0.5f, window.getSize().y * 0.6f }, { 200.f, -300.f })
        , m_baseView(window.getDefaultView())
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

        // grid generating
        m_blocks.generateResponsive(
            window,
            /*desiredRows*/ 6,
            /*targetBlockW*/ 72.f,
            /*targetBlockH*/ 28.f,
            /*gap*/ 8.f,
            /*topMargin*/ 60.f,
            /*sideMargin*/ 12.f,
            /*bottomSafety*/ 120.f);

        m_starfield.init(window.getSize(), /*count*/ 900);
    }

    void World::update(float dt, class InputSystem& input)
    {
        PROFILE_SCOPE("World::update");
        m_paddle.update(dt, input, m_window);
        m_ball.update(dt, m_window, m_paddle);

        handleBallBlocksCollision();

        // shake and background
        m_shake.update(dt);
        m_starfield.update(dt, m_window.getSize());
    }

    void World::render(sf::RenderTarget& rt)
    {
        PROFILE_SCOPE("World::render");

        // current view
        sf::View prevView;
        sf::Vector2f shakeOff{ 0.f, 0.f };

        if (auto* wnd = dynamic_cast<sf::RenderWindow*>(&rt))
        {
            prevView = wnd->getView();
            shakeOff = m_shake.offset();

            // stars paralax
            sf::View bg = prevView;
            bg.setCenter(prevView.getCenter() + shakeOff * 0.2f);
            wnd->setView(bg);
            m_starfield.render(rt);

            // shake
            sf::View fg = prevView;
            fg.setCenter(prevView.getCenter() + shakeOff);
            wnd->setView(fg);
        }

        rt.draw(m_boundsTop);
        rt.draw(m_boundsLeft);
        rt.draw(m_boundsRight);

        m_blocks.render(rt);
        m_paddle.render(rt);
        m_ball.render(rt);

        // return view
        if (auto* wnd = dynamic_cast<sf::RenderWindow*>(&rt))
        {
            wnd->setView(prevView);
        }
    }

    void World::resetBall()
    {
        m_ball.reset(
            { m_window.getSize().x * 0.5f, m_window.getSize().y * 0.6f },
            { 200.f, -300.f });
    }

    void World::handleBallBlocksCollision()
    {
        // colliders
        CollisionManifold man{};

        for (auto& b : m_blocks.blocks())
        {
            if (!b->alive())
                continue;

            // collide ball, block
            if (!m_ball.collider().test(b->collider(), man))
                continue;

            auto pos = m_ball.position();
            pos += man.normal * man.penetration;

            // reflect speed from normal
            auto vel = m_ball.velocity();
            float dot = vel.x * man.normal.x + vel.y * man.normal.y;
            vel = vel - 2.f * dot * man.normal;

            m_ball.reset(pos, vel);

            // damage to block
            if (b->alive())
                b->onHit();

            bool destroyedNow = !b->alive();

            // shake when destroy
            if (destroyedNow)
            {
                m_shake.trigger(/*mag*/4.0f, /*dur*/0.10f);
            }
            else
            {
                m_shake.trigger(/*mag*/1.8f, /*dur*/0.06f);
            }

            break;
        }
    }

} // namespace ark