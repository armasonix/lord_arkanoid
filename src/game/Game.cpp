#include "game/Game.h"
#include "game/GameState.h"
#include "core/Resources.h"
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>

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
            m_hint.setString("Use UP/DOWN to navigate, ENTER to select");
            m_hint.setCharacterSize(20);
            m_hint.setFillColor(sf::Color(220, 220, 220));

            m_status.setFont(font);
            m_status.setCharacterSize(18);
            m_status.setFillColor(sf::Color(200, 240, 200));
            m_status.setString("");

            m_options.clear();

            for (const auto* label : { "RESUME", "SAVE GAME" })
            {
                sf::Text t;
                t.setFont(font);
                t.setString(label);
                t.setCharacterSize(28);
                t.setFillColor(sf::Color(200, 200, 220));
                m_options.push_back(t);
            }

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
                m_hint.setPosition(cx, cy + 80.f);
            }

            const float startY = cy + 10.f;
            const float step = 40.f;
            for (std::size_t i = 0; i < m_options.size(); ++i)
            {
                auto b = m_options[i].getLocalBounds();
                m_options[i].setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_options[i].setPosition(cx, startY + step * static_cast<float>(i));
            }

            m_status.setPosition(cx, cy + 120.f);
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
            else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Up)
            {
                m_selected--;
                if (m_selected < 0)
                    m_selected = static_cast<int>(m_options.size()) - 1;
            }
            else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Down)
            {
                m_selected++;
                if (m_selected >= static_cast<int>(m_options.size()))
                    m_selected = 0;
            }
            else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
            {
                activateSelection();
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

            for (std::size_t i = 0; i < m_options.size(); ++i)
            {
                if (static_cast<int>(i) == m_selected)
                    m_options[i].setFillColor(sf::Color(255, 255, 180));
                else
                    m_options[i].setFillColor(sf::Color(200, 200, 220));

                rt.draw(m_options[i]);
            }

            if (!m_status.getString().isEmpty())
                rt.draw(m_status);
        }

    private:
        void activateSelection()
        {
            if (m_selected == 0)
            {
                m_game.togglePause();
            }
            else if (m_selected == 1)
            {
                if (m_game.saveToFile(DEFAULT_SAVE_FILE))
                {
                    m_status.setFillColor(sf::Color(200, 240, 200));
                    m_status.setString("Game saved");
                }
                else
                {
                    m_status.setFillColor(sf::Color(240, 200, 200));
                    m_status.setString("Save failed");
                }

                auto b = m_status.getLocalBounds();
                auto pos = m_status.getPosition();
                m_status.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                m_status.setPosition(pos);
            }
        }

        sf::Text m_title;
        sf::Text m_hint;
        sf::Text m_status;
        sf::RectangleShape m_overlay;
        std::vector<sf::Text> m_options;
        int m_selected{ 0 };
    };

    // game

    Game::Game(Context ctx)
        : m_ctx(ctx)
    {
        if (m_ctx.window)
        {
            m_world = std::make_unique<World>(*m_ctx.window, m_eventBus, m_scoreSystem);
        }

        changeState(std::make_unique<PlayingGameState>(*this));
        m_eventBus.addListener(&m_scoreSystem);
        m_scoreSystem.reset();
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

    int Game::lives() const
    {
        return m_world ? m_world->lives() : 0;
    }

    void Game::resetLevel()
    {
        if (!m_ctx.window)
            return;

        m_world = std::make_unique<World>(*m_ctx.window, m_eventBus, m_scoreSystem);
        changeState(std::make_unique<PlayingGameState>(*this));
    }

    bool Game::saveToFile(const std::string& path)
    {
        if (!m_world)
            return false;

        return m_world->saveToFile(path);
    }

    bool Game::loadFromFile(const std::string& path)
    {
        if (!m_world)
            return false;

        return m_world->loadFromFile(path);
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