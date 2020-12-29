#pragma once

#undef radians
#undef degrees

class Angle
{
public:

    Angle();

    float asDegrees() const;
    float asRadians() const;

private:

    friend Angle degrees(const float deg);
    friend Angle radians(const float rad);

	explicit Angle(const float rad);

private:

	float m_radians;
};

Angle degrees(const float deg);

Angle radians(const float rad);

bool operator ==(Angle left, Angle right);

bool operator !=(Angle left, Angle right);

bool operator <(Angle left, Angle right);

bool operator >(Angle left, Angle right);

bool operator <=(Angle left, Angle right);

bool operator >=(Angle left, Angle right);

Angle operator -(Angle right);

Angle operator +(Angle left, Angle right);

Angle& operator +=(Angle& left, Angle right);

Angle operator -(Angle left, Angle right);

Angle& operator -=(Angle& left, Angle right);

Angle operator *(Angle left, float right);

Angle operator *(float left, Angle right);

Angle& operator *=(Angle& left, float right);

Angle operator /(Angle left, float right);

Angle& operator /=(Angle& left, float right);

float operator /(Angle left, Angle right);

Angle operator %(Angle left, Angle right);

Angle& operator %=(Angle& left, Angle right);