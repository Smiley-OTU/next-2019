#include "stdafx.h"
#include "Line.h"
//Make this sentinel more obscure if we run into trouble.
#define DEFAULT_VALUE 9002.0f

const unsigned int CLine::s_components = 4;

CLine::CLine(const CPoint & a_p1, const CPoint & a_p2) :
	p1(a_p1), p2(a_p2)
{
}

CLine::CLine(float a_p1x, float a_p1y, float a_p2x, float a_p2y) :
	p1x(a_p1x), p1y(a_p1y), p2x(a_p2x), p2y(a_p2y)
{
}

//Not safe and not useful at the moment.
/*CLine::CLine(float a_values[4])
{
	memcpy(a_values, values, sizeof(float) * s_components);
}*/

CLine::CLine() : p1(CPoint{}), p2(CPoint{})
{
}

CLine::~CLine()
{
}

bool CLine::initialized() const
{
	return p1.initialized() && p2.initialized();
}

float CLine::operator[](unsigned int index) const
{
#if _DEBUG
	assert(index < s_components);
#endif
	return values[index];
}
