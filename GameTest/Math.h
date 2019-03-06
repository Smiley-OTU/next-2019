#pragma once
#include "stdafx.h"
#include "Point.h"
#include "Line.h"
#include "ColouredLine.h"

//Current log criteria.
#if _DEBUG
#define LOG true
#else
#define LOG true
#endif

//No matrices necessary for the ray caster / game in general cause no transformations other than translations (thus no need to concatenate transformations)! 
namespace Math {

	inline void print(const CPoint& point)
	{
#if LOG
		printf("x: %f, y: %f.\n", point.x, point.y);
#endif
	}

	inline void print(const CLine& line)
	{
#if LOG
		printf("p1x: %f, p1y: %f, p2x: %f, p2y %f.\n", line.p1x, line.p1y, line.p2x, line.p2y);
#endif
	}

	//a.b (2d vector dot product).
	inline float dot(const CPoint& a, const CPoint& b) {
		return a.x * b.x + a.y * b.y;
	}

	//axb (2d vector cross product).
	inline float cross(const CPoint& a, const CPoint& b) {
		return a.x * b.y - a.y * b.x;
	}

	//Manhattan distance of a vector.
	inline float l1norm(const CPoint& point) {
		return fabs(point.x) + fabs(point.y);
	}

	//Manhattan distance of a line.
	inline float l1norm(const CLine& line) {
		return fabs(line.p2y) - fabs(line.p1y) + fabs(line.p2x) - fabs(line.p1x);
	}

	//Euclidean distance of a vector.
	inline float l2norm(const CPoint& point) {
		return sqrtf(dot(point, point));
	}

	//Euclidean distance of a line.
	inline float l2norm(const CLine& line) {
		//cmath's implementation is stupid cause it does nested function calls and casting.
		float xDelta = line.p2x - line.p1x;
		float yDelta = line.p2y - line.p1y;
		return sqrtf(xDelta * xDelta + yDelta * yDelta);
	}

	//Projects A onto B, returning the length the projection.
	inline float projecf(const CPoint& a, const CPoint& b) {
		return dot(a, b) / l2norm(b);
	}

	//Projects A onto B, returning a point of length A in the direction of B.
	inline CPoint project(const CPoint& a, const CPoint& b) {
		return CPoint{ b * projecf(a, b) };
	}

	inline bool intersect(const CLine& a, const CLine& b, CPoint& poi) {
		const CPoint r{ a.p2.x - a.p1.x, a.p2.y - a.p1.y };
		const CPoint s{ b.p2.x - b.p1.x, b.p2.y - b.p1.y };

		float d = cross(r, s);

		//u and t are scalar values of parameterics
		float u = (cross(b.p1, r) - cross(a.p1, r)) / d;
		float t = (cross(b.p1, s) - cross(a.p1, s)) / d;

		if ((0 <= u) && (u <= 1) && (0 <= t) && (t <= 1)) {
			//Can't assign due to const so we must resort to hacks!
			//poi = CPoint{ r.x * t + a.p1.x, r.y * t + a.p1.y };
			CPoint tmp{ r.x * t + a.p1.x, r.y * t + a.p1.y };
			memmove(&poi, &tmp, sizeof(CPoint));

			//Kind of a bottleneck so I'll make a function that returns rather than overwrites.
			return true;
		}

		return false;
	}

	inline CPoint intersect(const CLine& a, const CLine& b) {
		const CPoint r{ a.p2.x - a.p1.x, a.p2.y - a.p1.y };
		const CPoint s{ b.p2.x - b.p1.x, b.p2.y - b.p1.y };

		float d = cross(r, s);

		//u and t are scalar values of parameterics
		float u = (cross(b.p1, r) - cross(a.p1, r)) / d;
		float t = (cross(b.p1, s) - cross(a.p1, s)) / d;

		//This is faster regardless of the memmove() due to no branch condition!
		return CPoint{ r.x * t + a.p1.x, r.y * t + a.p1.y };
	}

	//TODO: think of a *fast* way to see if there was an intersection.
	//Could return a "CIntersectResult", but that would be weird.
	//Let's see if we can get by without anything more. May be able to render regardless of if there's intersection!

}
