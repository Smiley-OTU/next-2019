#include "stdafx.h"
#include "Player.h"
#include "App/app.h"
#include "SimpleTileMap.h"
#include "App/SimpleController.h"

CPlayer::CPlayer() : m_translationSpeed(100.0f), m_rotationSpeed(100.0f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(const CSimpleTileMap& map, float deltaTime)
{
	//CSimpleControllers polls for 4 controllers and updates. GetController() defaults to the 0th controller.
	const CController& controller = CSimpleControllers::GetInstance().GetController();
	deltaTime /= 1000.0f;
	const float rotationVelocity = m_rotationSpeed * deltaTime;

	if(controller.CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
		m_angle -= rotationVelocity;
	else if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
		m_angle += rotationVelocity;

	const float translationVelocity = m_translationSpeed * deltaTime;
	const CPoint direction{ Math::direction(m_angle) };
	CPoint translation{ direction * translationVelocity };
	if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
		translation *= 1.0f;
	else if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
		translation *= -1.0f;
	else
		translation *= 0.0f;

	//Negate the translation component if it will move the player to an invalid tile.
	CPoint destination{ m_position + translation };
	EMapValue xTile = map.GetTileMapValue(destination.x, m_position.y);
	EMapValue yTile = map.GetTileMapValue(m_position.x, destination.y);
	if (yTile == EMapValue::BORDER || yTile == EMapValue::WALL || yTile == EMapValue::OUTOFBOUNDS)
		destination.y -= translation.y;
	if (xTile == EMapValue::BORDER || xTile == EMapValue::WALL || xTile == EMapValue::OUTOFBOUNDS)
		destination.x -= translation.x;
	m_position = destination;

	//Clamp the angle between 0 and 360 (prevents overflow and gives me nice numbers).
	m_angle += 360.0f;
	m_angle = fmod(m_angle, 360.0f);
}

float CPlayer::GetSpeed()
{
	return m_translationSpeed;
}
