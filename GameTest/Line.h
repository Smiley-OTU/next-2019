#pragma once
#include "Point.h"
struct CLine
{
	CLine(const CPoint& a_p1, const CPoint& a_p2);
	CLine(float a_px1, float a_p1y, float a_p2x, float a_p2y);
	//CLine(float a_values[4]);//Not safe and not useful at the moment.
	CLine();
	~CLine();

	//Lines are frequently created so it makes sense to have a mechanism to verify their status. 
	bool initialized() const;

	//Member variable access ie start y is[1], max of [3].
	float operator[](unsigned int index) const;

	//Immutable for now for defensive programming.
	union {
		struct {
			const CPoint p1, p2;
		};
		struct {
			const float p1x, p1y, p2x, p2y;
		};
		struct {
			const float values[4];
		};
	};

	const static unsigned int s_components;
};

