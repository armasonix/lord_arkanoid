#pragma once
#include <SFML/System.hpp>

namespace ark 
{

    class CameraShake 
    {
    public:
        void trigger(float magnitude, float duration) 
        {
            m_time = duration;
            m_mag = magnitude;
        }

        void update(float dt) 
        {
            if (m_time > 0.f) 
            {
                m_time -= dt;
                if (m_time < 0.f) m_time = 0.f;
            }
        }

        // cam offset in current frame
        sf::Vector2f offset() const 
        {
            if (m_time <= 0.f) return { 0.f, 0.f };

            float ox = (float(rand()) / RAND_MAX * 2.f - 1.f) * m_mag;
            float oy = (float(rand()) / RAND_MAX * 2.f - 1.f) * m_mag;
            return { ox, oy };
        }

        bool active() const { return m_time > 0.f; }

    private:
        float m_time{ 0.f };
        float m_mag{ 0.f };
    };

} // namespace ark