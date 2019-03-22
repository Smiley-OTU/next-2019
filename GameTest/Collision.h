#pragma once
#include "Circle.h"
#include "Box.h"
#include "Line.h"
//Minimum translation vector (mtv) is returned from all collide functions.
namespace Collision {
	CPoint collide(const CCircle& a, const CCircle& b);
	CPoint collide(const CCircle& a, const CBox& b);
	CPoint collide(const CCircle& a, const CLine& b);
	CPoint collide(const CBox& a, const CBox& b);
}