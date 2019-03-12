#pragma once
#include "Point.h"
class CEntity
{
public:
	CEntity();
	~CEntity();
private:
	CPoint m_position;
	float m_angle;
};

