#pragma once
#include <SFML/Graphics/Color.hpp>

namespace ark::gfx
{
    sf::Color hsvToRgb(float hDeg, float s, float v, sf::Uint8 alpha = 255);

    sf::Color rainbowColor(float time, float speed = 1.f, sf::Uint8 alpha = 255);
    sf::Color warmRainbowColor(float time, float speed = 1.f, sf::Uint8 alpha = 255);
    sf::Color coolRainbowColor(float time, float speed = 1.f, sf::Uint8 alpha = 255);
    sf::Color baseColorPulse(const sf::Color& base, float time, float speed = 1.f, float intensity = 0.25f, sf::Uint8 alpha = 255);

    sf::Uint8 pulseAlpha(float time, sf::Uint8 minAlpha = 120, sf::Uint8 maxAlpha = 255, float speed = 2.f);
}