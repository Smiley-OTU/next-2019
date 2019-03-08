#pragma once
#include "stdafx.h"
#include "Colour.h"
#include "App/AppSettings.h"
#define RAND_COL FRAND_RANGE(0.0f, 1.0f)

void CColour::Randomize()
{
	r = RAND_COL, g = RAND_COL, b = RAND_COL;
}

CColour CColour::Random()
{
	return CColour{ RAND_COL, RAND_COL, RAND_COL };
}
