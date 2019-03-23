#include "stdafx.h"
#include "Sprite.h"

CSprite::CSprite(float width, float height, float a_r, float a_g, float a_b) : m_width(width), m_height(height), r(a_r), g(a_g), b(a_b)
{
}

CSprite::CSprite() : r(1.0f), g(1.0f), b(1.0f), m_width(40.0f), m_height(30.0f)
{
}

CSprite::~CSprite()
{
}