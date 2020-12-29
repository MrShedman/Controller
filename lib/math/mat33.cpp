#include "mat33.h"
#include "quat.h"

#include <math.h>

Mat33::Mat33()
{

}

Mat33::Mat33(   const float xx, const float xy, const float xz,
                const float yx, const float yy, const float yz,
                const float zx, const float zy, const float zz)
{ 
    m[0] = Vec3(xx, xy, xz); 
    m[1] = Vec3(yx, yy, yz); 
    m[2] = Vec3(zx, zy, zz);
}

void Mat33::setRPY(const Vec3& euler)
{
    const float cx = cosf(euler.x);
    const float sx = sinf(euler.x);
    const float cy = cosf(euler.y);
    const float sy = sinf(euler.y); 
    const float cz = cosf(euler.z);
    const float sz = sinf(euler.z);

    const float cc = cx * cz;
    const float cs = cx * sz;
    const float sc = sx * cz;
    const float ss = sx * sz;

    m[0] = Vec3(cy * cz, sy * sc - cs, sy * cc + ss);
    m[1] = Vec3(cy * sz, sy * ss + cc, sy * cs - sc);
    m[2] = Vec3(-sy,     cy * sx,      cy * cx);
}

Vec3 Mat33::getRPY() const
{
    Vec3 euler;

    if (fabsf(m[2].x) >= 1.0)
    {
        euler.z = 0.0;

        // From difference of angles formula
        if (m[2].x < 0.0)  //gimbal locked down
        {
            euler.y = M_PI / 2.0;
            euler.x = atan2f(m[0].y, m[0].z);
        }
        else // gimbal locked up
        {
            euler.y = -M_PI / 2.0;
            euler.x = atan2f(-m[0].y, -m[0].z);
        }
    }
    else
    {
        euler.y = -asinf(m[2].x);
        euler.x = atan2f(m[2].y / cosf(euler.y), m[2].z / cosf(euler.y));
        euler.z = atan2f(m[1].x / cosf(euler.y), m[0].x / cosf(euler.y));
    }

    return euler;
}

void Mat33::setQuat(const Quat& q)
{
    const float d = dot(q, q);
    const float s = 2.0 / d;

    const float xs = q.x * s,   ys = q.y * s,   zs = q.z * s;
    const float wx = q.w * xs,  wy = q.z * ys,  wz = q.w * zs;
    const float xx = q.x * xs,  xy = q.x * ys,  xz = q.x * zs;
    const float yy = q.y * ys,  yz = q.y * zs,  zz = q.z * zs;

    m[0] = Vec3(1.0 - (yy + zz),    xy - wz,            xz + wy);
    m[1] = Vec3(xy + wz,            1.0 - (xx + zz),    yz - wx);
    m[2] = Vec3(xz - wy,            yz + wx,            1.0 - (xx + yy));
}

Mat33 identity()
{
    Mat33 m;
    m.m[0][0] = 1.0;
    m.m[1][1] = 1.0;
    m.m[2][2] = 1.0;
    return m;
}

float cofac(const Mat33& m, int r1, int c1, int r2, int c2)
{
    return m.m[r1][c1] * m.m[r2][c2] - m.m[r1][c2] * m.m[r2][c1];
}

float tdotx(const Mat33& m, const Vec3& v) 
{
    return m.m[0].x * v.x + m.m[1].x * v.y + m.m[2].x * v.z;
}
float tdoty(const Mat33& m, const Vec3& v) 
{
    return m.m[0].y * v.x + m.m[1].y * v.y + m.m[2].y * v.z;
}
float tdotz(const Mat33& m, const Vec3& v) 
{
    return m.m[0].z * v.x + m.m[1].z * v.y + m.m[2].z * v.z;
}

Vec3 operator*(const Mat33& m, const Vec3& v) 
{
	return Vec3(dot(m.m[0], v), dot(m.m[1], v), dot(m.m[2], v));
}

Mat33 operator*(const Mat33& lhs, const Mat33& rhs) 
{
    return Mat33(
		tdotx(rhs, lhs.m[0]), tdoty(rhs, lhs.m[0]), tdotz(rhs, lhs.m[0]),
		tdotx(rhs, lhs.m[1]), tdoty(rhs, lhs.m[1]), tdotz(rhs, lhs.m[1]),
		tdotx(rhs, lhs.m[2]), tdoty(rhs, lhs.m[2]), tdotz(rhs, lhs.m[2]));
}

Mat33& operator*=(Mat33& lhs, const Mat33& rhs) 
{
    lhs = lhs * rhs;
    return lhs;
}

Mat33 transpose(const Mat33& m)
{
	return Mat33(m.m[0].x, m.m[1].x, m.m[2].x,
		        m.m[0].y, m.m[1].y, m.m[2].y,
		        m.m[0].z, m.m[1].z, m.m[2].z);
}

float determinant(const Mat33& m)
{ 
	return triple(m.m[0], m.m[1], m.m[2]);
}

Mat33 inverse(const Mat33& m)
{
	const Vec3 co(cofac(m, 1, 1, 2, 2), cofac(m, 1, 2, 2, 0), cofac(m, 1, 0, 2, 1));
	const float det = dot(m.m[0], co);

	if (det != 0.0)
    {
        const float s = 1.0 / det;
        return Mat33(co.x * s, cofac(m, 0, 2, 2, 1) * s, cofac(m, 0, 1, 1, 2) * s,
                     co.y * s, cofac(m, 0, 0, 2, 2) * s, cofac(m, 0, 2, 1, 0) * s,
                     co.z * s, cofac(m, 0, 1, 2, 0) * s, cofac(m, 0, 0, 1, 1) * s);
    }
    else
    {
        return identity();
    }    
}