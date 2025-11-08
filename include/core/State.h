#pragma once
#include <SFML/Graphics.hpp>

struct Context 
{
    sf::RenderWindow* window{};
    class StateMachine* states{};
    class ResourceManager* resources{};
    class InputSystem* input{};
    class DebugOverlay* debug{};
};

class State 
{
public:
    explicit State(Context ctx) : m_ctx(ctx) {}
    virtual ~State() = default;

    // RAII: state resources
    virtual void onEnter() {}    // init
    virtual void onExit() {}     // release

    virtual void handleEvent(const sf::Event&) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget& rt) = 0;

protected:
    Context m_ctx;
};