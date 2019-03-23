#pragma once
#include "Point.h"
//Also give friendship to ghost behaviour or whatever so that ghosts can access colour to flicker when pacman gets a powerup.
class CRayCaster;
class CSprite
{
	friend CRayCaster;
public:
	CSprite(float width, float height, float a_r, float a_g, float a_b);
	CSprite();
	~CSprite();

	CPoint position;
private:
	float m_width;
	float m_height;
	float r, g, b;
};

