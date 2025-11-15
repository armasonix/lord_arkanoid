#include "game/Collision.h"
#include "game/Collidable.h"
#include <algorithm>
#include <cmath>

namespace ark
{
    static float clampf(float v, float lo, float hi)
    {
        return std::max(lo, std::min(v, hi));
    }

    bool circleIntersectsAabb(const Circle& cir,
        const AABB& box,
        sf::Vector2f& n,
        float& pen)
    {
        float cx = clampf(cir.c.x, box.pos.x, box.pos.x + box.size.x);
        float cy = clampf(cir.c.y, box.pos.y, box.pos.y + box.size.y);
        sf::Vector2f closest{ cx, cy };

        sf::Vector2f d = { cir.c.x - closest.x, cir.c.y - closest.y };
        float dist2 = d.x * d.x + d.y * d.y;
        if (dist2 > cir.r * cir.r)
            return false;

        float dist = std::sqrt(std::max(dist2, 0.00001f));
        n = (dist > 0.f)
            ? sf::Vector2f{ d.x / dist, d.y / dist }
        : sf::Vector2f{ 0.f, -1.f };

        pen = cir.r - dist;
        return true;
    }

    // collidable

    Collidable::Collidable(const GameObject& owner, AABB box)
        : m_owner(owner)
        , m_shape(ShapeKind::Box)
        , m_box(box)
        , m_cir{}
    {}

    Collidable::Collidable(const GameObject& owner, Circle cir)
        : m_owner(owner)
        , m_shape(ShapeKind::Disk)
        , m_box{}
        , m_cir(cir)
    {}

    AABB Collidable::aabb() const
    {
        if (m_shape == ShapeKind::Box)
            return m_box;

        return AABB
        {
            sf::Vector2f{ m_cir.c.x - m_cir.r, m_cir.c.y - m_cir.r },
            sf::Vector2f{ 2.f * m_cir.r, 2.f * m_cir.r }
        };
    }

    void Collidable::setBox(AABB b)
    {
        m_shape = ShapeKind::Box;
        m_box = b;
    }

    void Collidable::setCircle(Circle c)
    {
        m_shape = ShapeKind::Disk;
        m_cir = c;
    }

    ObjKind Collidable::ownerKind() const
    {
        return m_owner.kind();
    }

    bool Collidable::test(const ICollidable& other, CollisionManifold& out) const
    {
        const auto* o = dynamic_cast<const Collidable*>(&other);

        if (m_shape == ShapeKind::Disk && o && o->m_shape == ShapeKind::Box)
        {
            sf::Vector2f n;
            float pen = 0.f;
            if (!circleIntersectsAabb(m_cir, o->m_box, n, pen))
                return false;

            // circleIntersectsAabb normal from box to circle.
            out.normal = n;
            out.penetration = pen;
            return true;
        }

        // box vs disk
        if (m_shape == ShapeKind::Box && o && o->m_shape == ShapeKind::Disk)
        {
            sf::Vector2f n;
            float pen = 0.f;
            if (!circleIntersectsAabb(o->m_cir, m_box, n, pen))
                return false;

            // n from box to disk
            out.normal = -n;
            out.penetration = pen;
            return true;
        }

        // disk vs disk
        if (m_shape == ShapeKind::Disk && o && o->m_shape == ShapeKind::Disk)
        {
            sf::Vector2f d = m_cir.c - o->m_cir.c;
            float dist2 = d.x * d.x + d.y * d.y;
            float rSum = m_cir.r + o->m_cir.r;

            if (dist2 > rSum * rSum)
                return false;

            float dist = std::sqrt(std::max(dist2, 0.00001f));
            if (dist > 0.f)
                out.normal = sf::Vector2f{ d.x / dist, d.y / dist };
            else
                out.normal = sf::Vector2f{ 0.f, -1.f };

            out.penetration = rSum - dist;
            return true;
        }

        // box vs box / fallback: AABB vs AABB
        const AABB a = aabb();
        const AABB b = other.aabb();

        float overlapLeft = (a.pos.x + a.size.x) - b.pos.x;
        float overlapRight = (b.pos.x + b.size.x) - a.pos.x;
        float overlapTop = (a.pos.y + a.size.y) - b.pos.y;
        float overlapBottom = (b.pos.y + b.size.y) - a.pos.y;

        if (overlapLeft <= 0.f || overlapRight <= 0.f ||
            overlapTop <= 0.f || overlapBottom <= 0.f)
        {
            return false;
        }

        float penX = std::min(overlapLeft, overlapRight);
        float penY = std::min(overlapTop, overlapBottom);

        float centerAx = a.pos.x + a.size.x * 0.5f;
        float centerAy = a.pos.y + a.size.y * 0.5f;
        float centerBx = b.pos.x + b.size.x * 0.5f;
        float centerBy = b.pos.y + b.size.y * 0.5f;

        if (penX < penY)
        {
            float sx = centerAx - centerBx;
            out.normal = (sx >= 0.f) ? sf::Vector2f{ 1.f, 0.f }
            : sf::Vector2f{ -1.f, 0.f };
            out.penetration = penX;
        }
        else
        {
            float sy = centerAy - centerBy;
            out.normal = (sy >= 0.f) ? sf::Vector2f{ 0.f, 1.f }
            : sf::Vector2f{ 0.f, -1.f };
            out.penetration = penY;
        }

        return true;
    }

} // namespace ark