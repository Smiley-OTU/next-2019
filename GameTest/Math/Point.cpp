#include "Point.h"
//#include "stdafx.h"
#include <assert.h>
#include <cstdio>

//Current log criteria.
#if _DEBUG
#define LOG true
#else
#define LOG true
#endif

const unsigned int CPoint::s_amount = 2;

CPoint::CPoint()
{
}

CPoint::CPoint(float a_x, float a_y) : x(a_x), y(a_y)
{
}

CPoint::~CPoint()
{
}

float CPoint::operator[](unsigned int index)
{
#if _DEBUG
	assert(index < s_amount);
#endif
	return values[index];
}

void CPoint::print()
{
#if LOG
	printf("x: %f y: %f\n.", x, y);
#endif
}
