#pragma once
#include "Point.h"

class CEntity
{
public:
	CPoint GetPosition();
	CPoint GetDirection();

	void SetPosition(const CPoint& position);
	void SetPosition(float x, float y);

	void SetDirection(const CPoint& direction);
	void SetDirection(float angle);

protected:
	CPoint m_position;	// Position in screen space, relative to the origin.
	float m_angle;		// Rotation in degrees, relative to the origin.
};
