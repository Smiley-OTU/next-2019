#pragma once
#include "Point.h"

struct CLine
{
	CLine(const CPoint& a_p1, const CPoint& a_p2);
	CLine(float a_px1, float a_p1y, float a_p2x, float a_p2y);

	float operator[](size_t index) const;

	union {
		struct {
			CPoint p1, p2;
		};
		struct {
			float p1x, p1y, p2x, p2y;
		};
		struct {
			std::array<float, 4> values;
		};
	};
};
