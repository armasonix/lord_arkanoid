#pragma once
#include <SFML/Graphics.hpp>

namespace ark 
{
    class StateMachine;
    class ResourceManager;
    class InputSystem;
    class DebugOverlay;
    class MusicService;
    class SoundService;

struct Context 
{
    sf::RenderWindow* window{};
    StateMachine* states{};
    ResourceManager* resources{};
    InputSystem* input{};
    DebugOverlay* debug{};
    MusicService* music{};
    SoundService* sfx{};
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

} // namespace ark