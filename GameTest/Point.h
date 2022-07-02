#pragma once
#include <array>

struct CPoint
{
	CPoint(float a_x, float a_y);
	CPoint();

	CPoint operator+(const CPoint&) const;
	CPoint operator-(const CPoint&) const;
	CPoint operator*(float) const;
	CPoint operator/(float) const;

	void operator+=(const CPoint&);
	void operator-=(const CPoint&);
	void operator*=(float);
	void operator/=(float);

	float operator[](size_t index) const;

	union {
		struct { float x, y; };
		struct { std::array<float, 2> values; };
	};
};