#pragma once
#include <atomic>

struct CPoint;
struct CAtomicPoint
{
	CAtomicPoint(float a_x, float a_y);
	CAtomicPoint();
	CAtomicPoint(const CAtomicPoint& point);
	CAtomicPoint& operator=(const CAtomicPoint& point);

	CAtomicPoint& operator=(const CPoint& point);
	CPoint ToPoint();

	CAtomicPoint operator+(const CAtomicPoint&) const;
	CAtomicPoint operator-(const CAtomicPoint&) const;

	void operator+=(const CAtomicPoint&);
	void operator-=(const CAtomicPoint&);

	float operator[](size_t index) const;

	union {
		struct { std::atomic<float> x, y; };
		struct { std::array<std::atomic<float>, 2> values; };
	};

private:
	void Swap(const CAtomicPoint& point);
};

