#include "vec3.h"
#include "quat.h"
#include "angle.h"

#include <math.h>
#include <algorithm>

Vec3::Vec3()
:
x(0.0),
y(0.0),
z(0.0)
{}

Vec3::Vec3(const float x, const float y, const float z)
:
x(x),
y(y),
z(z)
{}

Vec3 polar(const Angle& angle, const float radius)
{
    return Vec3(cosf(angle.asRadians()), sinf(angle.asRadians()), 0.0) * radius;
}

float& Vec3::operator[](int index)
{
    switch (index)
    {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return x;
    }
}

const float& Vec3::operator[](int index) const
{
    switch (index)
    {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return x;
    }
}

Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vec3 operator*(const Vec3& lhs, const float rhs)
{
    return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vec3 operator/(const Vec3& lhs, const float rhs)
{
    return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

Vec3& operator+=(Vec3& lhs, const Vec3& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;

    return lhs;
}

Vec3& operator-=(Vec3& lhs, const Vec3& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;

    return lhs;
}

Vec3& operator*=(Vec3& lhs, const float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;

    return lhs;
}

Vec3& operator/=(Vec3& lhs, const float rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;

    return lhs;
}

bool operator==(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Vec3& lhs, const Vec3& rhs)
{
    return !operator==(lhs, rhs);
}

float dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3 cross(const Vec3& lhs, const Vec3& rhs)
{
    const float nx = lhs.y * rhs.z - lhs.z * rhs.y;
    const float ny = lhs.z * rhs.x - lhs.x * rhs.z;
    const float nz = lhs.x * rhs.y - lhs.y * rhs.x;

    return Vec3(nx, ny, nz);
}

float triple(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
    return dot(v1, cross(v2, v3));
}

float length_sq(const Vec3& v)
{
    return dot(v, v);
}

float length(const Vec3& v)
{
    return sqrtf(length_sq(v));
}

float distance(const Vec3& lhs, const Vec3& rhs)
{
    return length(rhs - lhs);
}

float distance_sq(const Vec3& lhs, const Vec3& rhs)
{
    return length_sq(rhs - lhs);
}

Vec3 min(const Vec3& lhs, const Vec3& rhs)
{
    Vec3 result;

    result.x = std::min(lhs.x, rhs.x);
    result.y = std::min(lhs.y, rhs.y);
    result.z = std::min(lhs.z, rhs.z);

    return result;
}

Vec3 max(const Vec3& lhs, const Vec3& rhs)
{
    Vec3 result;

    result.x = std::max(lhs.x, rhs.x);
    result.y = std::max(lhs.y, rhs.y);
    result.z = std::max(lhs.z, rhs.z);

    return result;
}

Vec3 normalise(const Vec3& v)
{
    return v / length(v);
}

Vec3 rotate(const Vec3& v, const Quat& rot)
{
	const Vec3 u(rot.x, rot.y, rot.z);
	const float s = rot.w;

	const Vec3 t = cross(u, v) * 2.0;
	return v + t * s + cross(u, t);
}

Angle angle_between(const Vec3& lhs, const Vec3& rhs)
{
    const float d = dot(lhs, rhs);
    const float mag_lhs = length(lhs);
    const float mag_rhs = length(rhs);
    return radians(acosf(d / (mag_lhs * mag_rhs)));
}