#include "AABB.h"

#include <limits>

AABB::AABB()
:
m_min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
m_max(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest())
{}

AABB::AABB(const std::vector<Vec3>& point_array)
:
AABB()
{
    include(point_array);
}

void AABB::include(const Vec3& point)
{
    m_min = ::min(m_min, point);
    m_max = ::max(m_max, point);
}

void AABB::include(const std::vector<Vec3>& point_array)
{
    for (auto &p : point_array)
    {
        include(p);
    }
}

bool AABB::contains(const Vec3& point)
{
    return (point.x > m_min.x && point.x < m_max.x &&
            point.y > m_min.y && point.y < m_max.y &&
            point.z > m_min.z && point.z < m_max.z);
}

const Vec3& AABB::min() const
{
    return m_min;
}

const Vec3& AABB::max() const
{
    return m_max;
}

float AABB::getWidth() const
{
    return m_max.x - m_min.x;
}

float AABB::getHeight() const
{
    return m_max.y - m_min.y;
}

float AABB::getDepth() const
{
    return m_max.z - m_min.z;
}

float AABB::getVolume() const
{
    return getWidth() * getHeight() * getDepth();
}