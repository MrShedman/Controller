#pragma once

class Quat;
class Angle;
class Vec3;

class Quat
{
public:

    Quat();

    Quat(const float x, const float y, const float z, const float w = 1.0);

    Quat(const Quat& axis, const Angle& angle);

    void setRPY(const Vec3& euler);

    float x;
    float y;
    float z;
    float w;
};

Quat operator+(const Quat& lhs, const Quat& rhs);

Quat operator-(const Quat& lhs, const Quat& rhs);

Quat operator*(const Quat& lhs, const float rhs);

Quat operator/(const Quat& lhs, const float rhs);

Quat& operator+=(Quat& lhs, const Quat& rhs);

Quat& operator-=(Quat& lhs, const Quat& rhs);

Quat& operator*=(Quat& lhs, const Quat& rhs);

Quat& operator*=(Quat& lhs, const float rhs);

Quat& operator/=(Quat& lhs, const float rhs);

float dot(const Quat& lhs, const Quat& rhs);

float length_sq(const Quat& q);

float length(const Quat& q);

Quat normalise(const Quat& q);

Quat lerp(const Quat& start, const Quat& end, const float factor);

Quat nlerp(const Quat& start, const Quat& end, const float factor);

Quat slerp(const Quat& start, const Quat& end, const float factor);