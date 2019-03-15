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

	inline void print(const CPoint& point, const std::string& message = "")
	{
#if LOG
		printf("%sx: %f, y: %f.\n", message.c_str(), point.x, point.y);
#endif
	}

	inline void print(const CLine& line, const std::string& message = "")
	{
#if LOG
		printf("%sp1x: %f, p1y: %f, p2x: %f, p2y %f.\n", message.c_str(), line.p1x, line.p1y, line.p2x, line.p2y);
#endif
	}

	inline float degrees(float radians) {
		return radians * 57.2957795130823f;
	}

	inline float radians(float degrees) {
		return degrees * 0.0174532925199f;
	}

	//Forms an angle measured in degrees relative to the origin based on the input vector in degrees.
	inline float angle(float dy, float dx) {
		return degrees(atan2f(radians(dy), radians(dx)));
	}

	//Forms an angle measured in degrees relative to the origin based on the input vector in degrees.
	inline float angle(const CPoint& direction) {
		return angle(direction.y, direction.x);
	}

	//Forms a two-component direction vector measured in degrees relative to the origin based on the input angle in radians.
	inline CPoint direction(float angle) {
		angle = radians(angle);
		//return CPoint{ degrees(cosf(angle)), degrees(sinf(angle)) };
		return CPoint{ cosf(angle), sinf(angle) };
	}

	//Forms an angle in radians relative to the origin based on the input vector in radians.
	inline float angle_r(float dy, float dx) {
		return atan2f(dy, dx);
	}

	//Forms an angle in radians relative to the origin based on the input vector in radians.
	inline float angle_r(const CPoint& direction) {
		return angle_r(direction.y, direction.x);
	}

	//Forms a two-component direction vector in radians relative to the origin based on the input angle in radians.
	inline CPoint direction_r(float angle) {
		return CPoint{ cosf(angle), sinf(angle) };
	}

	//Convert value between (-1.0, 1.0f) to (0.0, 1.0).
	inline float bias(float value) {
		return (value + 1.0f) * 0.5f;
	}

	//Convert value between (0.0, 1.0f) to (-1.0, 1.0).
	inline float unbias(float value) {
		return value * 2.0f - 1.0f;
	}

	//Converts a value from its original range to the desire range (think lerp).
	inline float map(float value, float inMin, float inMax, float outMin, float outMax) {
		return ((value - inMin) / (inMax - inMin)) * (outMax - outMin) + outMin;
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
	inline float l1norm(float x, float y) {
		return fabs(x) + fabs(y);
	}

	//Manhattan distance of a vector.
	inline float l1norm(const CPoint& point) {
		return l1norm(point.x, point.y);
	}

	//Manhattan distance of a line.
	inline float l1norm(const CLine& line) {
		return l1norm(line.p2x - line.p1x, line.p2y - line.p1y);
	}

	//Euclidean distance of a vector.
	inline float l2norm(float x, float y) {
		return sqrtf(x * x + y * y);
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

	//Creates a point with a length of one in the direciton of the passed in point.
	inline CPoint normalize(const CPoint& point) {
		return CPoint{ point / l2norm(point) };
	}

	//Creates a point with a length of one in the direciton of the passed in line.
	inline CPoint normalize(const CLine& line) {
		CPoint delta = line.p2 - line.p1;
		return CPoint{ delta / l2norm(delta) };
	}

	//Projects A onto B, returning the length the projection.
	inline float projecf(const CPoint& a, const CPoint& b) {
		return dot(a, b) / l2norm(b);
	}

	//Projects A onto B, returning a point of length A in the direction of B.
	inline CPoint project(const CPoint& a, const CPoint& b) {
		return CPoint{ b * projecf(a, b) };
	}

	//Writes the point of intersection to the output point (poi), returns a boolean to indicate intersection.
	inline bool intersect(const CLine& a, const CLine& b, CPoint& poi) {
		const CPoint r{ a.p2.x - a.p1.x, a.p2.y - a.p1.y };
		const CPoint s{ b.p2.x - b.p1.x, b.p2.y - b.p1.y };

		float d = cross(r, s);

		//u and t are scalar values of parameterics
		float u = (cross(b.p1, r) - cross(a.p1, r)) / d;
		float t = (cross(b.p1, s) - cross(a.p1, s)) / d;

		if ((0 <= u) && (u <= 1) && (0 <= t) && (t <= 1)) {
			poi = CPoint{ r.x * t + a.p1.x, r.y * t + a.p1.y };
			return true;
		}

		return false;
	}

	//Returns the point of intersection between lines a and b.
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
