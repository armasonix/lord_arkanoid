#include "states/GameOverState.h"
#include "core/StateMachine.h"
#include "core/Resources.h"
#include "states/MenuState.h"

namespace ark
{

void GameOverState::onEnter() 
{
    auto& font = m_ctx.resources->font("mono");
    m_text.setFont(font);
    m_text.setString("GAME OVER\nPress Enter to menu");
    m_text.setCharacterSize(28);
    m_text.setFillColor(sf::Color(255, 120, 120));
    m_text.setPosition(80.f, 140.f);
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
void GameOverState::render(sf::RenderTarget& rt) { rt.draw(m_text); }

} // namespace ark