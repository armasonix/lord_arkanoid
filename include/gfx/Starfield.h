#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>

namespace ark::gfx 
{

    class Starfield 
    {
    public:
        // sizeMin/MaxPx 
        void init(const sf::Vector2u& size, std::size_t count = 900,
            float sizeMinPx = 0.8f, float sizeMaxPx = 1.6f)
        {
            m_rng.seed(std::random_device{}());
            m_mesh.setPrimitiveType(sf::Quads);
            m_mesh.resize(count * 4);
            m_stars.resize(count);

            std::uniform_real_distribution<float> rx(0.f, static_cast<float>(size.x));
            std::uniform_real_distribution<float> ry(0.f, static_cast<float>(size.y));
            std::uniform_real_distribution<float> spd(12.f, 38.f);          // slow flowing down
            std::uniform_real_distribution<float> baseB(0.25f, 1.0f);       // basic brightness
            std::uniform_real_distribution<float> amp(0.08f, 0.35f);        // twinkle amplitude
            std::uniform_real_distribution<float> freq(0.6f, 1.6f);         // twinkle frequancy
            std::uniform_real_distribution<float> phase(0.f, 6.2831853f);   // phase offset
            m_sizeMin = sizeMinPx;
            m_sizeMax = std::max(sizeMinPx, sizeMaxPx);

            for (std::size_t i = 0; i < count; ++i) 
            {
                auto& s = m_stars[i];
                s.pos = { rx(m_rng), ry(m_rng) };
                s.speed = spd(m_rng);
                s.base = baseB(m_rng);
                s.amp = amp(m_rng);
                s.freq = freq(m_rng);
                s.ph = phase(m_rng);
                // linking size with brightness: brighter = bigger
                s.size = lerp(m_sizeMin, m_sizeMax, s.base);

                rebuildQuad(i, s.pos, s.size, brightnessToColor(s.base));
            }
        }

        void update(float dt, const sf::Vector2u& size)
        {
            m_time += dt;
            for (std::size_t i = 0; i < m_stars.size(); ++i) 
            {
                auto& s = m_stars[i];
                s.pos.y += s.speed * dt;
                if (s.pos.y > static_cast<float>(size.y)) {
                    s.pos.y = -1.f;
                    s.pos.x = std::uniform_real_distribution<float>(0.f, static_cast<float>(size.x))(m_rng);
                }
                // twinkle
                float bri = std::clamp(s.base + s.amp * std::sin(s.ph + s.freq * m_time), 0.f, 1.f);
                auto col = brightnessToColor(bri);
                rebuildQuad(i, s.pos, s.size, col);
            }
        }

        void render(sf::RenderTarget& rt) const 
        {
            rt.draw(m_mesh);
        }

    private:
        struct Star 
        {
            sf::Vector2f pos;
            float speed;
            float base, amp, freq, ph;
            float size;
        };

        static sf::Color brightnessToColor(float b) 
        {
            const sf::Uint8 c = static_cast<sf::Uint8>(std::clamp(b, 0.f, 1.f) * 255.f);
            return sf::Color(c, c, c, 255);
        }

        static float lerp(float a, float b, float t) { return a + (b - a) * t; }

        void rebuildQuad(std::size_t idx, const sf::Vector2f& p, float sizePx, const sf::Color& col) 
        {
            const float hs = sizePx * 0.5f;
            const std::size_t v = idx * 4;

            m_mesh[v + 0].position = { p.x - hs, p.y - hs };
            m_mesh[v + 1].position = { p.x + hs, p.y - hs };
            m_mesh[v + 2].position = { p.x + hs, p.y + hs };
            m_mesh[v + 3].position = { p.x - hs, p.y + hs };
            m_mesh[v + 0].color = m_mesh[v + 1].color = m_mesh[v + 2].color = m_mesh[v + 3].color = col;
        }

        std::vector<Star>  m_stars;
        sf::VertexArray    m_mesh;   // quads
        std::mt19937       m_rng;
        float              m_time{ 0.f };
        float              m_sizeMin{ 1.6f }, m_sizeMax{ 2.6f };
    };

} // namespace ark::gfx