#include "stdafx.h"
#include "Point.h"
#include "Math.h"

CPoint::CPoint() :
	x(0.0f), y(0.0f)
{
}

CPoint::CPoint(float a_x, float a_y) :
	x(a_x), y(a_y)
{
}

CPoint CPoint::operator+(const CPoint &p) const
{
	return CPoint{ x + p.x, y + p.y };
}

CPoint CPoint::operator-(const CPoint &p) const
{
	return CPoint{ x - p.x, y - p.y };
}

CPoint CPoint::operator*(float s) const
{
	return CPoint( x * s, y * s );
}

CPoint CPoint::operator/(float s) const
{
	return CPoint{ x / s, y / s };
}

void CPoint::operator+=(const CPoint &p)
{
	x += p.x;
	y += p.y;
}

void CPoint::operator-=(const CPoint &p)
{
	x -= p.x;
	y -= p.y;
}

void CPoint::operator*=(float s)
{
	x *= s;
	y *= s;
}

void CPoint::operator/=(float s)
{
	x /= s;
	y /= s;
}

float CPoint::operator[](size_t index) const
{
	return values[index];
}