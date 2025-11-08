#pragma once
#include <SFML/Graphics.hpp>

namespace ark::gfx 
{

    // base color effect interface
    class ColorEffect 
    {
    public:
        virtual ~ColorEffect() = default;
        virtual sf::Color apply(const sf::Color& base, float saturation01) const = 0;
    };

    // sat=1 full color, sat=0 grey
    class DesaturateEffect final : public ColorEffect 
    {
    public:
        sf::Color apply(const sf::Color& base, float s) const override 
        {
            s = std::clamp(s, 0.f, 1.f);
            // luminance grey = 0.2126r + 0.7152g + 0.0722b
            float r = base.r / 255.f, g = base.g / 255.f, b = base.b / 255.f;
            float grey = 0.2126f * r + 0.7152f * g + 0.0722f * b;
            float rr = grey * (1.f - s) + r * s;
            float gg = grey * (1.f - s) + g * s;
            float bb = grey * (1.f - s) + b * s;
            auto clamp = [](float v) { return (sf::Uint8)std::clamp(v * 255.f, 0.f, 255.f); };
            return sf::Color{ clamp(rr), clamp(gg), clamp(bb), base.a };
        }
    };

} // namespace ark::gfx