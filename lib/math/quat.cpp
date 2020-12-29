#include "quat.h"

#include "vec3.h"
#include "angle.h"

#include <math.h>

Quat::Quat()
:
x(0.0),
y(0.0),
z(0.0),
w(1.0)
{}

Quat::Quat(const float x, const float y, const float z, const float w)
:
x(x),
y(y),
z(z),
w(w)
{}

Quat::Quat(const Quat& axis, const Angle& angle)
{
    const float sin_half = sinf(angle.asRadians() * 0.5);
    const float cos_half = cosf(angle.asRadians() * 0.5);

    x = axis.x * sin_half;
    y = axis.y * sin_half;
    z = axis.z * sin_half;
    w = cos_half;
}

void Quat::setRPY(const Vec3& euler)
{
    const float c1 = cosf(euler.x * 0.5);
    const float c2 = cosf(euler.y * 0.5);
    const float c3 = cosf(euler.z * 0.5);
    
    const float s1 = sinf(euler.x * 0.5);
    const float s2 = sinf(euler.y * 0.5);       
    const float s3 = sinf(euler.z * 0.5);

    w = c1 * c2 * c3 - s1 * s2 * s3;
    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
}

Quat operator+(const Quat& lhs, const Quat& rhs)
{
    return Quat(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

Quat operator-(const Quat& lhs, const Quat& rhs)
{
    return Quat(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

Quat operator*(const Quat& lhs, const Quat& rhs)
{
    const float nw = (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z);
    const float nx = (lhs.x * rhs.w) + (lhs.w * rhs.x) + (lhs.y * rhs.z) - (lhs.z * rhs.y);
    const float ny = (lhs.y * rhs.w) + (lhs.w * rhs.y) + (lhs.z * rhs.x) - (lhs.x * rhs.z);
    const float nz = (lhs.z * rhs.w) + (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x);

    return Quat(nx, ny, nz, nw);
}

Quat operator/(const Quat& lhs, const float rhs)
{
    return Quat(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

Quat& operator+=(Quat& lhs, const Quat& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;

    return lhs;
}

Quat& operator-=(Quat& lhs, const Quat& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;

    return lhs;
}

Quat& operator*=(Quat& lhs, const Quat& rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

Quat& operator*=(Quat& lhs, const float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    lhs.w *= rhs;

    return lhs;
}

Quat& operator/=(Quat& lhs, const float rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    lhs.w /= rhs;

    return lhs;
}

float dot(const Quat& lhs, const Quat& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

float length_sq(const Quat& q)
{
    return dot(q, q);
}

float length(const Quat& q)
{
    return sqrtf(length_sq(q));
}

Quat normalise(const Quat& q)
{
    const float l = length(q);
    return Quat(q.x / l, q.y / l, q.z / l, q.w / l);
}

Quat lerp(const Quat& start, const Quat& end, const float factor)
{
    return (start + (end - start) * factor);
}

Quat nlerp(const Quat& start, const Quat& end, const float factor)
{
    Quat corrected_end;

    if (dot(start, end) < 0.0)
    {
        corrected_end = end * -1.0;
    }
    else
    {
        corrected_end = end;
    }

    return normalise(lerp(start, corrected_end, factor));
}

Quat slerp(const Quat& start, const Quat& end, const float factor)
{
    const float EPSILON = 1e-3;
    const float dot_ = dot(start, end);

    if (fabsf(dot_) > (1.0 - EPSILON)) 
    {
        return nlerp(start, end, factor);
    }

    const float theta_0 = acosf(dot_);
    const float theta = theta_0 * factor;

    Quat v2 = end - start * dot_;
    normalise(v2);

    return start * cosf(theta) + v2 * sin(theta);
}