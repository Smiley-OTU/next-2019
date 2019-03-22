#include "stdafx.h"
#include "Collision.h"
#include "Math.h"

CPoint Collision::collide(const CCircle & a, const CCircle & b)
{
	CPoint difference{ a.position - b.position };
	float radiiSum = a.radius + b.radius;
	if (Math::l1norm(difference) > radiiSum)
		return CPoint{ 0.0f, 0.0f };

	return CPoint();
}

CPoint Collision::collide(const CCircle & a, const CBox & b)
{
	return CPoint();
}

CPoint Collision::collide(const CCircle & a, const CLine & b)
{
	return CPoint();
}

CPoint Collision::collide(const CBox & a, const CBox & b)
{
	return CPoint();
}