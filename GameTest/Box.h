#pragma once
#include "Point.h"
namespace Collision {
	//Technically this has the same information as a line, but the typing makes it clear this is for collision.
	struct CBox
	{
		CBox();
		~CBox();

		union {
			struct {
				float xMin, yMin, xMax, yMax;
			};
			struct {
				CPoint min, max;
			};
		};
	};
}