#include "game/Game.h"
#include "game/GameState.h"
#include "core/Resources.h"
#include <SFML/Graphics.hpp>
#include <utility>

namespace ark
{

    // playing
    class PlayingGameState final : public GameState
    {
    public:
        using GameState::GameState;

        void handleEvent(const sf::Event& e) override
        {
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
            {
                m_game.togglePause();
            }
        }

        void update(float dt) override
        {
            auto* world = m_game.worldPtr();
            auto& ctx = m_game.context();
            if (!world || !ctx.input)
                return;

            world->update(dt, *ctx.input);
        }

        void render(sf::RenderTarget& rt) override
        {
            auto* world = m_game.worldPtr();
            if (!world)
                return;
            world->render(rt);
        }
    };

    // pause
    class PausedGameState final : public GameState
    {
    public:
        using GameState::GameState;

        void onEnter() override
        {
            auto& ctx = m_game.context();
            if (!ctx.window || !ctx.resources)
                return;

            auto& font = ctx.resources->font("mono");

            m_title.setFont(font);
            m_title.setString("PAUSED");
            m_title.setCharacterSize(48);
            m_title.setFillColor(sf::Color(240, 240, 240));

            m_hint.setFont(font);
            m_hint.setString("Press ESC to resume");
            m_hint.setCharacterSize(24);
            m_hint.setFillColor(sf::Color(220, 220, 220));

            auto sz = ctx.window->getSize();
            float cx = static_cast<float>(sz.x) * 0.5f;
            float cy = static_cast<float>(sz.y) * 0.5f;

            {
                auto b = m_title.getLocalBounds();
                m_title.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_title.setPosition(cx, cy - 40.f);
            }

            {
                auto b = m_hint.getLocalBounds();
                m_hint.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_hint.setPosition(cx, cy + 20.f);
            }

            m_overlay.setSize(sf::Vector2f(static_cast<float>(sz.x), static_cast<float>(sz.y)));
            m_overlay.setPosition(0.f, 0.f);
            m_overlay.setFillColor(sf::Color(0, 0, 0, 140));
        }

        void handleEvent(const sf::Event& e) override
        {
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
            {
                m_game.togglePause();
            }
        }

        void update(float) override
        {}

        void render(sf::RenderTarget& rt) override
        {
            auto* world = m_game.worldPtr();
            if (world)
                world->render(rt);

            rt.draw(m_overlay);
            rt.draw(m_title);
            rt.draw(m_hint);
        }

    private:
        sf::Text m_title;
        sf::Text m_hint;
        sf::RectangleShape m_overlay;
    };

    // game

    Game::Game(Context ctx)
        : m_ctx(ctx)
    {
        if (m_ctx.window)
        {
            m_world = std::make_unique<World>(*m_ctx.window);
        }

        changeState(std::make_unique<PlayingGameState>(*this));
    }

    void Game::changeState(std::unique_ptr<GameState> state)
    {
        if (m_state)
            m_state->onExit();

        m_state = std::move(state);

        if (m_state)
            m_state->onEnter();
    }

    void Game::handleEvent(const sf::Event& e)
    {
        if (m_state)
            m_state->handleEvent(e);
    }

    void Game::update(float dt)
    {
        if (m_state)
            m_state->update(dt);
    }

    void Game::render(sf::RenderTarget& rt)
    {
        if (m_state)
            m_state->render(rt);
    }

    bool Game::isBallLost() const
    {
        return m_world && m_world->isBallLost();
    }

    bool Game::victory() const
    {
        return m_world && m_world->victory();
    }

    float Game::ballSpeed() const
    {
        return m_world ? m_world->ballSpeed() : 0.f;
    }

    void Game::resetLevel()
    {
        if (!m_ctx.window)
            return;

        m_world = std::make_unique<World>(*m_ctx.window);
        changeState(std::make_unique<PlayingGameState>(*this));
    }

    void Game::togglePause()
    {
        if (!m_state)
            return;

        if (dynamic_cast<PlayingGameState*>(m_state.get()) != nullptr)
        {
            changeState(std::make_unique<PausedGameState>(*this));
        }
        else if (dynamic_cast<PausedGameState*>(m_state.get()) != nullptr)
        {
            changeState(std::make_unique<PlayingGameState>(*this));
        }
    }

    bool Game::isPaused() const
    {
        return dynamic_cast<PausedGameState*>(m_state.get()) != nullptr;
    }

    World& Game::world()
    {
        return *m_world;
    }

    const World& Game::world() const
    {
        return *m_world;
    }

    World* Game::worldPtr()
    {
        return m_world.get();
    }

    const World* Game::worldPtr() const
    {
        return m_world.get();
    }

    Context& Game::context()
    {
        return m_ctx;
    }

    const Context& Game::context() const
    {
        return m_ctx;
    }

} // namespace ark