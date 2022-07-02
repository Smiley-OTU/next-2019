#include "stdafx.h"
#include "Line.h"

CLine::CLine(const CPoint & a_p1, const CPoint & a_p2) :
	p1(a_p1), p2(a_p2)
{
}

CLine::CLine(float a_p1x, float a_p1y, float a_p2x, float a_p2y) :
	p1x(a_p1x), p1y(a_p1y), p2x(a_p2x), p2y(a_p2y)
{
}

float CLine::operator[](size_t index) const
{
	return values[index];
}
