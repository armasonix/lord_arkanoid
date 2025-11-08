#pragma once
#include <SFML/System.hpp>

struct Circle { sf::Vector2f c; float r; };
struct AABB { sf::Vector2f pos; sf::Vector2f size; };

bool circleIntersectsAabb(const Circle& cir, const AABB& box, sf::Vector2f& outNormal, float& outPenetration);