#include "states/GameOverState.h"
#include "core/StateMachine.h"
#include "core/Resources.h"
#include "states/MenuState.h"
#include "audio/MusicService.h"

namespace ark
{
    void GameOverState::onEnter()
    {
        auto& font = m_ctx.resources->font("mono");
        m_text.setFont(font);
        m_text.setString("GAME OVER\nPress Enter to menu");
        m_text.setCharacterSize(32);
        m_text.setFillColor(sf::Color(255, 120, 120));

        if (m_ctx.window)
        {
            auto sz = m_ctx.window->getSize();
            float cx = static_cast<float>(sz.x) * 0.5f;
            float cy = static_cast<float>(sz.y) * 0.5f;
            auto b = m_text.getLocalBounds();
            m_text.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
            m_text.setPosition(cx, cy);
        }

        if (m_ctx.music)
            m_ctx.music->stop();
    }

    void GameOverState::handleEvent(const sf::Event& e)
    {
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
        {
            m_ctx.states->clear();
            m_ctx.states->push<MenuState>();
        }
    }

    void GameOverState::update(float) {}

    void GameOverState::render(sf::RenderTarget& rt)
    {
        rt.draw(m_text);
    }

} // namespace ark