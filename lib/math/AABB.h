#pragma once

#include "vec3.h"

#include <vector>

class AABB
{
public:

	AABB();
    AABB(const std::vector<Vec3>& point_array);

	void include(const Vec3& point);
    void include(const std::vector<Vec3>& point_array);

    bool contains(const Vec3& point);

    const Vec3& min() const;
    const Vec3& max() const;

	float getWidth() const;
	float getHeight() const;
	float getDepth() const;
	float getVolume() const;

private:

	Vec3 m_min;
	Vec3 m_max;
};