#include "stdafx.h"
#include "Player.h"
#include "App/app.h"
#include "SimpleTileMap.h"

CPlayer::CPlayer() : m_translationSpeed(100.0f), m_rotationSpeed(100.0f), m_radius(20.0f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(const CSimpleTileMap& map, float deltaTime)
{
	deltaTime /= 1000.0f;
	const float translationVelocity = m_translationSpeed * deltaTime;
	const float rotationVelocity = m_rotationSpeed * deltaTime;

	if(App::IsKeyPressed(Keys::A))
		m_angle -= rotationVelocity;
	else if (App::IsKeyPressed(Keys::D))
		m_angle += rotationVelocity;

	CPoint translation{ Math::direction(m_angle) * translationVelocity };
	if (App::IsKeyPressed(Keys::W))
		m_position += translation;
	else if (App::IsKeyPressed(Keys::S))
		m_position -= translation;

	//Clamp the angle between 0 and 360 (prevents overflow and gives me nice numbers).
	m_angle += 360.0f;
	m_angle = fmod(m_angle, 360.0f);
}

float CPlayer::GetRadius()
{
	return m_radius;
}
