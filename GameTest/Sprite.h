#pragma once
#include "Point.h"
#include <array>

class CRayCaster;
class CSprite
{
	friend CRayCaster;
public:
	CSprite(float width, float height, float a_r, float a_g, float a_b);
	CSprite();
	~CSprite();

	CPoint position;
	float r, g, b;
private:
	float m_width;
	float m_height;
};

using Sprites = std::array<CSprite, 4>;