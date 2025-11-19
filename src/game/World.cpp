#include "game/World.h"
#include "utils/Profiler.h"
#include "core/SaveManager.h"
#include "core/GameSave.h"
#include <iostream>
#include <new>

namespace ark
{

    World::World(sf::RenderWindow& window, GameEventBus& events)
        : m_window(window)
        , m_events(events)
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

        m_lifeSave = createSave();
        m_hasLifeSave = true;
        m_gameOver = false;
    }

    void World::update(float dt, class InputSystem& input)
    {
        PROFILE_SCOPE("World::update");

        m_paddle.update(dt, input, m_window);
        m_ball.update(dt, m_window, m_paddle);

        handleBallBlocksCollision();

        updateLifeSystem();

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

            // stars parallax
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
        CollisionManifold man{};

        for (auto& b : m_blocks.blocks())
        {
            if (!b->alive())
                continue;

            if (!m_ball.collider().test(b->collider(), man))
                continue;

            auto pos = m_ball.position();
            pos += man.normal * man.penetration;

            auto vel = m_ball.velocity();
            float dot = vel.x * man.normal.x + vel.y * man.normal.y;
            vel = vel - 2.f * dot * man.normal;
            m_ball.reset(pos, vel);

            if (b->alive())
                b->onHit();

            bool destroyedNow = !b->alive();

            if (destroyedNow)
            {
                GameEvent ev;
                ev.type = GameEventType::BlockDestroyed;

                switch (b->type())
                {
                case BlockType::Low: ev.blockKind = BlockKind::Normal; break;
                case BlockType::Med: ev.blockKind = BlockKind::Hard; break;
                case BlockType::Hev: ev.blockKind = BlockKind::Bonus; break;
                default:             ev.blockKind = BlockKind::Unknown;
                }

                m_events.dispatch(ev);
            }

            // shake
            if (destroyedNow)
            {
                m_shake.trigger(4.0f, 0.10f);
            }
            else
            {
                m_shake.trigger(1.8f, 0.06f);
            }
            break;
        }
    }

    void World::updateLifeSystem()
    {
        if (m_gameOver)
            return;

        if (!m_ball.fellBelow())
        {
            m_lifeSave = createSave();
            m_hasLifeSave = true;
            return;
        }

        if (m_lives > 0 && m_hasLifeSave)
        {
            --m_lives;

            GameSave s = m_lifeSave;
            s.lives = m_lives;

            applySave(s);
            return;
        }

        if (m_lives <= 0)
        {
            m_gameOver = true;
        }
    }

    bool World::isBallLost() const
    {
        return m_gameOver;
    }

    GameSave World::createSave() const
    {
        GameSave s;

        // position and speed
        s.ballPosition = m_ball.position();
        s.ballVelocity = m_ball.velocity();
        s.paddlePosition = m_paddle.position();

        // blocks
        for (auto& b : m_blocks.blocks())
        {
            BlockSave bs;
            bs.position = b->position();
            bs.destroyed = !b->alive();
            s.blocks.push_back(bs);
        }

        s.score = m_score;
        s.lives = m_lives;

        return s;
    }

    void World::applySave(const GameSave& s)
    {
        // ball
        m_ball.reset(s.ballPosition, s.ballVelocity);

        {
            const sf::Vector2f paddleSize = m_paddle.shape().getSize();
            m_paddle.~Paddle();
            new (&m_paddle) Paddle(paddleSize, s.paddlePosition);
        }

        m_score = s.score;
        m_lives = s.lives;

        auto& blocks = m_blocks.blocks();
        for (std::size_t i = 0; i < blocks.size() && i < s.blocks.size(); ++i)
        {
            const auto& saved = s.blocks[i];
            auto& b = blocks[i];

            const bool shouldBeDestroyed = saved.destroyed;
            const bool isAlive = b->alive();

            if (shouldBeDestroyed && isAlive)
            {
                while (b->alive())
                    b->onHit();
            }
            else if (!shouldBeDestroyed && !isAlive)
            {
                if (auto* block = dynamic_cast<Block*>(b.get()))
                {
                    block->restore();
                }
            }
        }
    }

    bool World::saveToFile(const std::string& path)
    {
        const GameSave s = createSave();
        return SaveManager::save(s, path);
    }

    bool World::loadFromFile(const std::string& path)
    {
        GameSave s;
        if (!SaveManager::load(s, path))
            return false;

        applySave(s);

        m_lifeSave = createSave();
        m_hasLifeSave = true;
        m_gameOver = false;

        return true;
    }

} // namespace ark