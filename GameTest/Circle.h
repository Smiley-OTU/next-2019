#pragma once
#include "Point.h"
namespace Collision {
	struct CCircle
	{
		CCircle();
		~CCircle();

		CPoint position;
		float radius;
	};
}
