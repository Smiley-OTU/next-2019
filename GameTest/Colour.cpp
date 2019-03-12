#pragma once
#include "stdafx.h"
#include "Colour.h"
#include "App/AppSettings.h"
#define RAND_COL FRAND_RANGE(0.0f, 1.0f)

const unsigned char CColour::s_components = 4;

CColour::CColour() : r(1.0f), g(1.0f), b(1.0f), a(1.0f)
{
}

CColour::CColour(float a_r, float a_g, float a_b) : r(a_r), g(a_g), b(a_b), a(1.0f)
{
}

void CColour::Randomize()
{
	r = RAND_COL, g = RAND_COL, b = RAND_COL;
}

float CColour::operator[](unsigned char index)
{
#if _DEBUG
	assert(index < s_components);
#endif
	return values[index];
}

CColour CColour::Random()
{
	return CColour{ RAND_COL, RAND_COL, RAND_COL };
}
