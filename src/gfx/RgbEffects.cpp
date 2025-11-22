#include "gfx/RgbEffects.h"
#include <algorithm>
#include <cmath>

namespace ark::gfx
{
    namespace
    {
        constexpr float TAU = 6.28318530718f;

        float clamp01(float v)
        {
            return std::clamp(v, 0.f, 1.f);
        }
    }

    sf::Color hsvToRgb(float hDeg, float s, float v, sf::Uint8 alpha)
    {
        hDeg = std::fmod(hDeg, 360.f);
        if (hDeg < 0.f)
            hDeg += 360.f;

        s = clamp01(s);
        v = clamp01(v);

        float c = v * s;
        float x = c * (1.f - std::fabsf(std::fmod(hDeg / 60.f, 2.f) - 1.f));
        float m = v - c;

        float r{}, g{}, b{};
        if (hDeg < 60.f) { r = c; g = x; b = 0.f; }
        else if (hDeg < 120.f) { r = x; g = c; b = 0.f; }
        else if (hDeg < 180.f) { r = 0.f; g = c; b = x; }
        else if (hDeg < 240.f) { r = 0.f; g = x; b = c; }
        else if (hDeg < 300.f) { r = x; g = 0.f; b = c; }
        else { r = c; g = 0.f; b = x; }

        auto toByte = [](float f)
            {
                return static_cast<sf::Uint8>(std::clamp(f * 255.f, 0.f, 255.f));
            };

        return sf::Color(toByte(r + m), toByte(g + m), toByte(b + m), alpha);
    }

    sf::Color rainbowColor(float time, float speed, sf::Uint8 alpha)
    {
        const float hue = std::fmod(time * speed * 120.f, 360.f);
        return hsvToRgb(hue, 0.9f, 1.f, alpha);
    }

    sf::Color warmRainbowColor(float time, float speed, sf::Uint8 alpha)
    {
        const float hueSpanStart = 10.f;
        const float hueSpanEnd = 55.f;
        const float normalized = 0.5f + 0.5f * std::sinf(time * speed * TAU * 0.08f);
        const float hue = hueSpanStart + (hueSpanEnd - hueSpanStart) * normalized;
        return hsvToRgb(hue, 0.85f, 1.f, alpha);
    }

    sf::Color coolRainbowColor(float time, float speed, sf::Uint8 alpha)
    {
        const float hueSpanStart = 185.f;
        const float hueSpanEnd = 225.f;
        const float normalized = 0.5f + 0.5f * std::sinf(time * speed * TAU * 0.08f + 1.2f);
        const float hue = hueSpanStart + (hueSpanEnd - hueSpanStart) * normalized;
        return hsvToRgb(hue, 0.75f, 1.f, alpha);
    }

    sf::Color baseColorPulse(const sf::Color& base, float time, float speed, float intensity, sf::Uint8 alpha)
    {
        intensity = clamp01(intensity);
        const float wave = 0.5f + 0.5f * std::sinf(time * speed * TAU * 0.16f);
        const float mix = 1.f - intensity + intensity * wave;

        auto mixChannel = [mix](sf::Uint8 c)
            {
                return static_cast<sf::Uint8>(std::clamp(static_cast<float>(c) * mix, 0.f, 255.f));
            };

        return sf::Color(mixChannel(base.r), mixChannel(base.g), mixChannel(base.b), alpha);
    }

    sf::Uint8 pulseAlpha(float time, sf::Uint8 minAlpha, sf::Uint8 maxAlpha, float speed)
    {
        const float minF = std::min(minAlpha, maxAlpha);
        const float maxF = std::max(minAlpha, maxAlpha);
        const float wave = 0.5f + 0.5f * std::sinf(time * speed * TAU * 0.25f);
        return static_cast<sf::Uint8>(minF + (maxF - minF) * wave);
    }
} // namespace ark