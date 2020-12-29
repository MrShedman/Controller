#include "angle.h"

#include <math.h>

Angle::Angle()
:
m_radians(0.0)
{

}

Angle::Angle(float rad)
:
m_radians(rad)
{

}

float Angle::asDegrees() const
{
    return m_radians * 180.0 / M_PI;
}

float Angle::asRadians() const
{
    return m_radians;
}

Angle degrees(const float deg)
{
	// Normalize to [0, 360]
    float new_deg = fmodf(deg, 360.0);
    if (new_deg < 0.0)
        new_deg += 360.0;

    return Angle(new_deg * M_PI / 180.0);
}

Angle radians(const float rad)
{
    return Angle(rad);
}

bool operator ==(Angle left, Angle right)
{
	return left.asRadians() == right.asRadians();
}

bool operator !=(Angle left, Angle right)
{
	return left.asRadians() != right.asRadians();
}

bool operator <(Angle left, Angle right)
{
	return left.asRadians() < right.asRadians();
}

bool operator >(Angle left, Angle right)
{
	return left.asRadians() > right.asRadians();
}

bool operator <=(Angle left, Angle right)
{
	return left.asRadians() <= right.asRadians();
}

bool operator >=(Angle left, Angle right)
{
	return left.asRadians() >= right.asRadians();
}

Angle operator -(Angle right)
{
	return radians(-right.asRadians());
}

Angle operator +(Angle left, Angle right)
{
	return radians(left.asRadians() + right.asRadians());
}

Angle& operator +=(Angle& left, Angle right)
{
	return left = left + right;
}

Angle operator -(Angle left, Angle right)
{
	return radians(left.asRadians() - right.asRadians());
}

Angle& operator -=(Angle& left, Angle right)
{
	return left = left - right;
}

Angle operator *(Angle left, float right)
{
	return radians(left.asRadians() * right);
}

Angle operator *(float left, Angle right)
{
	return right * left;
}

Angle& operator *=(Angle& left, float right)
{
	return left = left * right;
}

Angle operator /(Angle left, float right)
{
	return radians(left.asRadians() / right);
}

Angle& operator /=(Angle& left, float right)
{
	return left = left / right;
}

float operator /(Angle left, Angle right)
{
	return left.asRadians() / right.asRadians();
}

Angle operator %(Angle left, Angle right)
{
	return radians(fmodf(left.asRadians(), right.asRadians()));
}

Angle& operator %=(Angle& left, Angle right)
{
	return left = left % right;
}