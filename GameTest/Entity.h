#pragma once
#include "Point.h"
class CEntity
{
public:
	CEntity();
	~CEntity();

	CPoint GetPosition();
	CPoint GetDirection();

	void SetPosition(const CPoint& position);
	void SetPosition(float x, float y);

	void SetDirection(const CPoint& direction);
	void SetDirection(float angle);

protected:
	//Position in screen space, relative to the origin.
	CPoint m_position;
	//Rotation in degrees, relative to the origin.
	float m_angle;
};

