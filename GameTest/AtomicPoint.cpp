#include "stdafx.h"
#include "AtomicPoint.h"
#include "Point.h"
#include "Math.h"

CAtomicPoint::CAtomicPoint() :
	x(0.0f), y(0.0f)
{
}

CAtomicPoint::CAtomicPoint(float a_x, float a_y) :
	x(a_x), y(a_y)
{
}

CAtomicPoint::CAtomicPoint(const CAtomicPoint& point)
{
	Swap(point);
}

CAtomicPoint& CAtomicPoint::operator=(const CAtomicPoint& point)
{
	Swap(point);
	return *this;
}

CAtomicPoint& CAtomicPoint::operator=(const CPoint& point)
{
	x = point.x;
	y = point.y;
}

CPoint CAtomicPoint::ToPoint()
{
	return { x, y };
}

CAtomicPoint CAtomicPoint::operator+(const CAtomicPoint& p) const
{
	return CAtomicPoint{ x + p.x, y + p.y };
}

CAtomicPoint CAtomicPoint::operator-(const CAtomicPoint& p) const
{
	return CAtomicPoint{ x - p.x, y - p.y };
}

void CAtomicPoint::operator+=(const CAtomicPoint& p)
{
	x += p.x;
	y += p.y;
}

void CAtomicPoint::operator-=(const CAtomicPoint& p)
{
	x -= p.x;
	y -= p.y;
}

float CAtomicPoint::operator[](size_t index) const
{
	return values[index];
}

void CAtomicPoint::Swap(const CAtomicPoint& point)
{
	x.store(point.x);
	y.store(point.y);
}
