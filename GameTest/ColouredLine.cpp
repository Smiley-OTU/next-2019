#include "stdafx.h"
#include "ColouredLine.h"
//Make this sentinel more obscure if we run into trouble.
#define DEFAULT_VALUE 9003.0f
//This one will be okay cause there is no good reason for colours to be greater than 1.0.
#define DEFAULT_COLOUR 9004.0f

const unsigned int CColouredLine::s_components = 8;

CColouredLine::CColouredLine(float a_p1x, float a_p1y, float a_p2x, float a_p2y, float a_r, float a_g, float a_b, float a_a) :
	p1x(a_p1x), p1y(a_p1y), p2x(a_p2x), p2y(a_p2y), r(a_r), g(a_g), b(a_b), a(a_a)
{
}

CColouredLine::CColouredLine(const CPoint & a_p1, const CPoint & a_p2, float a_r, float a_g, float a_b, float a_a) :
	p1(a_p1), p2(a_p1), r(a_r), g(a_g), b(a_b), a(a_a)
{
}

CColouredLine::CColouredLine(const CLine & a_line, float a_r, float a_g, float a_b, float a_a) :
	line(a_line), r(a_r), g(a_g), b(a_b), a(a_a)
{
}

/*CColouredLine::CColouredLine(float values[8])
{
	memcpy(a_values, values, sizeof(float) * s_components);
}*/

CColouredLine::CColouredLine() : line(CLine{}), r(DEFAULT_COLOUR), g(DEFAULT_COLOUR), b(DEFAULT_COLOUR), a(DEFAULT_COLOUR)
{
}

CColouredLine::~CColouredLine()
{
}

bool CColouredLine::initialized()
{
	return line.initialized() && r != DEFAULT_COLOUR && b != DEFAULT_COLOUR && g != DEFAULT_COLOUR && a != DEFAULT_COLOUR;
}

float CColouredLine::operator[](unsigned int index) const
{
#if _DEBUG
	assert(index < s_components);
#endif
	return values[index];
}