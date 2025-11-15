#include "game/Game.h"
#include "game/GameState.h"
#include <utility> 

namespace ark
{
    class PlayingGameState final : public GameState
    {
    public:
        using GameState::GameState;

        void handleEvent(const sf::Event& e) override
        {
            (void)e;
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

    // game

    Game::Game(Context ctx)
        : m_ctx(ctx)
    {
        if (m_ctx.window)
        {
            m_world = std::make_unique<World>(*m_ctx.window);
        }
        // playing state
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