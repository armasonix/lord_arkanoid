#include "states/PauseState.h"
#include "core/StateMachine.h"
#include "core/Resources.h"

namespace ark
{

void PauseState::onEnter() 
{
    auto& font = m_ctx.resources->font("mono");
    m_text.setFont(font);
    m_text.setString("PAUSED\nPress ESC to resume");
    m_text.setCharacterSize(28);
    m_text.setFillColor(sf::Color::Yellow);
    m_text.setPosition(80.f, 120.f);
}

void PauseState::handleEvent(const sf::Event& e) 
{
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) 
    {
        m_ctx.states->pop();
    }
}

void PauseState::update(float) {}

void PauseState::render(sf::RenderTarget& rt) { rt.draw(m_text); }

} // namespace ark