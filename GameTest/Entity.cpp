#include "stdafx.h"
#include "Entity.h"

CPoint CEntity::GetPosition()
{
	return m_position;
}

CPoint CEntity::GetDirection()
{
	return Math::direction(m_angle);
}

void CEntity::SetPosition(const CPoint & position)
{
	m_position = position;
}

void CEntity::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void CEntity::SetDirection(const CPoint & direction)
{
	m_angle = Math::angle(direction);
}

void CEntity::SetDirection(float angle)
{
	m_angle = angle;
}