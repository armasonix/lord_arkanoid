#include "core/Application.h"
#include "states/BootState.h"
#include "utils/Profiler.h"

namespace ark
{

Application::Application(unsigned w, unsigned h, const char* title)
    : m_window(sf::VideoMode(w, h), title, sf::Style::Close)
    , m_debug(&m_resources.font("mono", std::string(ASSETS_DIR) + "/fonts/Roboto-Regular.ttf"))
    , m_states(Context{ &m_window, &m_states, &m_resources, &m_input, &m_debug })
{
    m_window.setFramerateLimit(0);
    m_window.setVerticalSyncEnabled(true);

    // init state
    m_states.push<BootState>();
    m_states.apply();
}

Application::~Application()
{
    m_states.clear();
    m_states.apply();
}

void Application::run() 
{
    const float dt = 1.f / 120.f;
    sf::Clock clock;
    float accumulator = 0.f;

    while (m_window.isOpen()) 
    {
        PROFILE_SCOPE("Frame");
        m_input.newFrame();

        // events processing
        {
            PROFILE_SCOPE("Events");
            sf::Event e{};
            while (m_window.pollEvent(e)) 
            {
                if (e.type == sf::Event::Closed) m_window.close();
                m_input.handleEvent(e, m_window);
                m_states.handleEvent(e);
            }
        }

        // clock
        float frame = clock.restart().asSeconds();
        accumulator += std::min(frame, 0.25f);

        // update
        while (accumulator >= dt) 
        {
            PROFILE_SCOPE("Update");
            m_states.update(dt);
            accumulator -= dt;
        }

        // render
        {
            PROFILE_SCOPE("Render");
            m_window.clear(sf::Color(18, 22, 28));
            m_debug.setFps(m_fps.fps());
            m_states.render(m_window);
            m_fps.frameEnd();
            m_lastFps = m_fps.fps();

            if (auto* dbg = &m_debug; dbg && dbg->visible()) 
            {}
            m_window.display();
        }

        m_fps.frameStart();
        m_states.apply();
    }
}

} // namespace ark