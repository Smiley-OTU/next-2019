#include "stdafx.h"
#include "Point.h"
#include "Math.h"
//Make this sentinel more obscure if we run into trouble.
#define DEFAULT_VALUE 9001.0f

const unsigned int CPoint::s_components = 2;

//*NOTE* Constructor doesn't actually get called when using list initialization ie Point{1, 2}.
CPoint::CPoint(float a_x, float a_y) :
	x(a_x), y(a_y)
{
}

//Not safe and not useful at the moment.
/*CPoint::CPoint(float a_values[2])
{
	memcpy(a_values, values, sizeof(float) * s_components);
}*/

CPoint::CPoint() : x(DEFAULT_VALUE), y(DEFAULT_VALUE)
{
}

CPoint::~CPoint()
{
}

bool CPoint::initialized() const
{
	return x != DEFAULT_VALUE && y != DEFAULT_VALUE;
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

/*void CPoint::operator+=(const CPoint &p)
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
	x *= s;
	y *= s;
}*/

float CPoint::operator[](unsigned int index) const
{
#if _DEBUG
	assert(index < s_components);
#endif
	return values[index];
}