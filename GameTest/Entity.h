#pragma once
#include "Point.h"
class CEntity
{
public:
	CEntity();
	~CEntity();

	void setPosition(const CPoint& position);
	void setPosition(float x, float y);

	void setDirection(const CPoint& direction);
	void setDirection(float angle);

protected:
	//Position in screen space, relative to the origin.
	CPoint m_position;
	//Rotation in degrees, relative to the origin.
	float m_angle;
};

