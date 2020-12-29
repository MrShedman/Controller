#pragma once

#include "vec3.h"

class Quat;

class Mat33
{
public:

    Mat33();

    Mat33(  const float xx, const float xy, const float xz,
            const float yx, const float yy, const float yz,
            const float zx, const float zy, const float zz);

    void setRPY(const Vec3& euler);

    Vec3 getRPY() const;

    void setQuat(const Quat& q);

    Vec3 m[3];
};

Mat33 identity();

float cofac(const Mat33& m, int r1, int c1, int r2, int c2);

float tdotx(const Mat33& m, const Vec3& v);

float tdoty(const Mat33& m, const Vec3& v);

float tdotz(const Mat33& m, const Vec3& v);

Vec3 operator*(const Mat33& m, const Vec3& v);

Mat33 operator*(const Mat33& lhs, const Mat33& rhs);

Mat33& operator*=(Mat33& lhs, const Mat33& rhs);

Mat33 transpose(const Mat33& m);

float determinant(const Mat33& m);

Mat33 inverse(const Mat33& m);