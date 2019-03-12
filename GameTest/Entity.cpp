#include "stdafx.h"
#include "Entity.h"
#include "Math.h"

CEntity::CEntity()
{
}

CEntity::~CEntity()
{
}

void CEntity::setPosition(const CPoint & position)
{
	m_position = position;
}

void CEntity::setPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void CEntity::setDirection(const CPoint & direction)
{
	m_angle = Math::angle(direction);
}

void CEntity::setDirection(float angle)
{
	m_angle = angle;
}