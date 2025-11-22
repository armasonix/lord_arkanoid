#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <algorithm>

namespace ark::gfx
{
    class TwinkleStarfield
    {
    public:
        void init(const sf::Vector2u& size, std::size_t count = 650,
            float sizeMinPx = 0.8f, float sizeMaxPx = 1.8f)
        {
            m_rng.seed(std::random_device{}());
            m_mesh.setPrimitiveType(sf::Quads);
            m_mesh.resize(count * 4);
            m_stars.resize(count);
            m_bounds = size;

            std::uniform_real_distribution<float> rx(0.f, static_cast<float>(size.x));
            std::uniform_real_distribution<float> ry(0.f, static_cast<float>(size.y));
            std::uniform_real_distribution<float> baseB(0.25f, 1.0f);
            std::uniform_real_distribution<float> amp(0.08f, 0.35f);
            std::uniform_real_distribution<float> freq(0.8f, 1.8f);
            std::uniform_real_distribution<float> phase(0.f, 6.2831853f);
            std::uniform_real_distribution<float> depth(0.4f, 1.f);
            std::uniform_real_distribution<float> wobbleAmp(2.5f, 12.f);
            std::uniform_real_distribution<float> wobbleSpeed(0.08f, 0.22f);
            m_sizeMin = sizeMinPx;
            m_sizeMax = std::max(sizeMinPx, sizeMaxPx);

            for (std::size_t i = 0; i < count; ++i)
            {
                auto& s = m_stars[i];
                s.pos = { rx(m_rng), ry(m_rng) };
                s.base = baseB(m_rng);
                s.amp = amp(m_rng);
                s.freq = freq(m_rng);
                s.ph = phase(m_rng);
                s.depth = depth(m_rng);
                s.wobbleAmp = wobbleAmp(m_rng) * s.depth;
                s.wobbleSpeed = wobbleSpeed(m_rng);
                s.wobblePhase = phase(m_rng);
                s.size = lerp(m_sizeMin, m_sizeMax, s.base);

                rebuildQuad(i, s.pos, s.size, brightnessToColor(s.base));
            }
        }

        void update(float dt)
        {
            m_time += dt;
            for (std::size_t i = 0; i < m_stars.size(); ++i)
            {
                auto& s = m_stars[i];
                float bri = std::clamp(s.base + s.amp * std::sin(s.ph + s.freq * m_time), 0.f, 1.f);
                auto col = brightnessToColor(bri);

                float wobbleX = std::sin(s.wobblePhase + s.wobbleSpeed * m_time) * s.wobbleAmp;
                float wobbleY = std::cos(s.wobblePhase * 0.8f + s.wobbleSpeed * 1.3f * m_time) * s.wobbleAmp * 0.35f;
                sf::Vector2f pos = s.pos + sf::Vector2f{ wobbleX, wobbleY };
                wrapPosition(pos);

                rebuildQuad(i, pos, s.size, col);
            }
        }

        void render(sf::RenderTarget& rt) const
        {
            rt.draw(m_mesh);
        }

    private:
        struct Star
        {
            sf::Vector2f pos{};
            float base{};
            float amp{};
            float freq{};
            float ph{};
            float depth{};
            float wobbleAmp{};
            float wobbleSpeed{};
            float wobblePhase{};
            float size{};
        };

        static sf::Color brightnessToColor(float b)
        {
            const sf::Uint8 c = static_cast<sf::Uint8>(std::clamp(b, 0.f, 1.f) * 255.f);
            return sf::Color(c, c, c, 255);
        }

        void wrapPosition(sf::Vector2f& p)
        {
            if (p.x < 0.f) p.x += static_cast<float>(m_bounds.x);
            if (p.x > static_cast<float>(m_bounds.x)) p.x -= static_cast<float>(m_bounds.x);
            if (p.y < 0.f) p.y += static_cast<float>(m_bounds.y);
            if (p.y > static_cast<float>(m_bounds.y)) p.y -= static_cast<float>(m_bounds.y);
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
        sf::VertexArray    m_mesh;
        std::mt19937       m_rng;
        sf::Vector2u       m_bounds{};
        float              m_time{ 0.f };
        float              m_sizeMin{ 1.2f }, m_sizeMax{ 2.2f };
    };

} // namespace ark::gfx