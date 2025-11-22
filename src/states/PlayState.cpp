#include "states/PlayState.h"
#include "states/WinState.h"
#include "states/EnterNameState.h"
#include "states/GameOverState.h"
#include "core/StateMachine.h"
#include "core/Resources.h"
#include "utils/DebugOverlay.h"
#include "input/InputSystem.h"
#include "audio/SoundService.h"
#include "audio/MusicService.h"

namespace
{
    constexpr const char* QUICK_SAVE_FILE = "savegame.dat";
}

namespace ark
{

    void PlayState::onEnter()
    {
        // create Game
        m_game = std::make_unique<Game>(m_ctx);

        if (m_loadSavedGame)
        {
            if (m_game->loadFromFile(m_initialSavePath))
            {
                m_goQueued = false;
                m_winQueued = false;
            }
        }

        if (m_ctx.music)
            m_ctx.music->playLevel(true);

        m_goQueued = false;
        m_winQueued = false;

        // ingame score
        auto& font = m_ctx.resources->font("mono");
        m_scoreText.setFont(font);
        m_scoreText.setCharacterSize(20);
        m_scoreText.setFillColor(sf::Color(240, 240, 240));
        m_scoreText.setString("Score: 0");
    }

    void PlayState::handleEvent(const sf::Event& e)
    {
        if (!m_game)
            return;

        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::F5)
            {
                m_game->saveToFile(DEFAULT_SAVE_FILE);
            }
            else if (e.key.code == sf::Keyboard::F9)
            {
                if (m_game->loadFromFile(DEFAULT_SAVE_FILE))
                {
                    m_goQueued = false;
                    m_winQueued = false;
                }
            }
        }

        m_game->handleEvent(e);
    }

    void PlayState::update(float dt)
    {
        m_dtLast = dt;

        if (m_game)
            m_game->update(dt);

        // lose
        if (!m_goQueued && m_game && m_game->isBallLost())
        {
            if (m_ctx.music) m_ctx.music->stop();
            if (m_ctx.sfx)   m_ctx.sfx->playEnsure(Sfx::Lose);

            int points = m_game->score().score();
            m_ctx.states->push<EnterNameState>(points, false);
            m_goQueued = true;
            return;
        }

        // win
        if (!m_winQueued && m_game && m_game->victory())
        {
            if (m_ctx.music) m_ctx.music->stop();

            int points = m_game->score().score();
            m_ctx.states->push<EnterNameState>(points, true);
            m_winQueued = true;
            return;
        }

        // debug overlay
        if (m_ctx.input && m_ctx.input->debugTogglePressed())
        {
            m_ctx.debug->setVisible(!m_ctx.debug->visible());
        }
    }

    void PlayState::render(sf::RenderTarget& rt)
    {
        if (m_game)
            m_game->render(rt);

        // score update
        if (m_game)
        {
            int points = m_game->score().score();
            m_scoreText.setString("Score: " + std::to_string(points));

            if (m_ctx.window)
            {
                auto sz = m_ctx.window->getSize();
                auto bounds = m_scoreText.getLocalBounds();

                m_scoreText.setOrigin(bounds.left + bounds.width, bounds.top);
                m_scoreText.setPosition(static_cast<float>(sz.x) - 10.f, 10.f);
            }

            rt.draw(m_scoreText);

            const int lives = m_game->lives();
            if (lives > 0)
            {
                constexpr float LIFE_RADIUS = 8.f;
                constexpr float LIFE_SPACING = 6.f;
                constexpr float LIFE_OFFSET_X = 10.f;
                constexpr float LIFE_OFFSET_Y = 10.f;

                sf::CircleShape lifeShape(LIFE_RADIUS);
                lifeShape.setFillColor(sf::Color(220, 40, 40));
                lifeShape.setOutlineColor(sf::Color(120, 15, 15));
                lifeShape.setOutlineThickness(1.f);

                for (int i = 0; i < lives; ++i)
                {
                    const float x = LIFE_OFFSET_X + static_cast<float>(i) * (LIFE_RADIUS * 2.f + LIFE_SPACING);
                    lifeShape.setPosition(x, LIFE_OFFSET_Y);
                    rt.draw(lifeShape);
                }
            }
        }

        if (m_ctx.debug->visible())
        {
            const float speed = m_game ? m_game->ballSpeed() : 0.f;
            m_ctx.debug->draw(rt, m_ctx.debug->fps(), m_dtLast, speed);
        }
    }

} // namespace ark