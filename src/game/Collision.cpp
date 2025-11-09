#include "game/Collision.h"
#include <algorithm>
#include <cmath>

namespace ark 
{
    static float clampf(float v, float lo, float hi) { return std::max(lo, std::min(v, hi)); }

    bool circleIntersectsAabb(const Circle& cir, const AABB& box,
        sf::Vector2f& n, float& pen) 
    {
        // find closest point of the rect to center of the circle
        float cx = clampf(cir.c.x, box.pos.x, box.pos.x + box.size.x);
        float cy = clampf(cir.c.y, box.pos.y, box.pos.y + box.size.y);
        sf::Vector2f closest{ cx, cy };
        sf::Vector2f d = { cir.c.x - closest.x, cir.c.y - closest.y };
        float dist2 = d.x * d.x + d.y * d.y;
        if (dist2 > cir.r * cir.r) return false;

        float dist = std::sqrt(std::max(dist2, 0.00001f));
        n = (dist > 0.f) ? sf::Vector2f{ d.x / dist, d.y / dist } : sf::Vector2f{ 0.f, -1.f };
        pen = cir.r - dist;
        return true;
    }
} // namespace ar