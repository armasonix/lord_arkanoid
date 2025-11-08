#include "core/StateMachine.h"
#include "core/State.h"
#include <SFML/Graphics.hpp>

StateMachine::StateMachine(Context ctx) : m_ctx(ctx) {}

void StateMachine::pop() { m_ops.push_back({ OpType::Pop, {} }); }
void StateMachine::clear() { m_ops.push_back({ OpType::Clear, {} }); }

void StateMachine::apply() 
{
    for (auto& op : m_ops) 
    {
        switch (op.type) 
        {
        case OpType::Push: 
        {
            auto st = op.factory(m_ctx);
            st->onEnter();
            m_stack.emplace_back(std::move(st));
        } break;
        case OpType::Pop: 
        {
            if (!m_stack.empty()) 
            {
                m_stack.back()->onExit();
                m_stack.pop_back();
            }
        } break;
        case OpType::Clear: 
        {
            while (!m_stack.empty()) 
            {
                m_stack.back()->onExit();
                m_stack.pop_back();
            }
        } break;
        }
    }
    m_ops.clear();
}

void StateMachine::handleEvent(const sf::Event& e) 
{
    if (!m_stack.empty()) m_stack.back()->handleEvent(e);
}

void StateMachine::update(float dt) 
{
    if (!m_stack.empty()) m_stack.back()->update(dt);
}

void StateMachine::render(sf::RenderTarget& rt) 
{
    if (!m_stack.empty()) m_stack.back()->render(rt);
}