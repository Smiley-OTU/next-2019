#include "stdafx.h"
#include "Player.h"
#include "App/app.h"
#include "SimpleTileMap.h"
#include "App/SimpleController.h"

CPlayer::CPlayer() : m_translationSpeed(100.0f), m_rotationSpeed(100.0f), m_radius(20.0f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(const CSimpleTileMap& map, float deltaTime)
{
	//Main polls for 4 controllers and updates.
	const CController& controller = CSimpleControllers::GetInstance().GetController();
	deltaTime /= 1000.0f;
	const float rotationVelocity = m_rotationSpeed * deltaTime;

	//if(App::IsKeyPressed(Keys::A))
	if(controller.CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
		m_angle += rotationVelocity;
	//else if (App::IsKeyPressed(Keys::D))
	else if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
		m_angle -= rotationVelocity;

	const float translationVelocity = m_translationSpeed * deltaTime;
	const CPoint direction{ Math::direction(m_angle) };
	CPoint translation{ direction * translationVelocity };
	//if (App::IsKeyPressed(Keys::W))
	if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
		translation *= 1.0f;
	//else if (App::IsKeyPressed(Keys::S))
	else if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
		translation *= -1.0f;
	else
		translation *= 0.0f;

	//printf("px: %f py: %f dx: %f dy: %f ox: %f oy: %f.\n", m_position.x, m_position.y, destination.x, destination.y, Math::overlap(destination.x, direction.x, map.getTileWidth()), Math::overlap(destination.y, direction.y, map.getTileHeight()));
	CPoint destination{ m_position + translation };
	EMapValue destinationValue = map.GetTileMapValue(destination.x, destination.y);
	if (destinationValue == EMapValue::BORDER || destinationValue == EMapValue::WALL || destinationValue == EMapValue::OUTOFBOUNDS) {
		//Broken ricochet algorithm :(
		/*const float tileWidth = map.getTileWidth();
		const float tileHeight = map.getTileHeight();

		const int xIndex = direction.x >= 0.0f ? floor(destination.x / tileWidth) : ceil(destination.x / tileWidth);
		const int yIndex = direction.y >= 0.0f ? floor(destination.y / tileHeight) : ceil(destination.y / tileHeight);

		const float xEdge = xIndex * tileWidth;
		const float yEdge = yIndex * tileHeight;

		float xTranslation = destination.x - xEdge;
		float yTranslation = destination.y - yEdge;

		destination.y -= yTranslation;
		destination.x -= xTranslation;*/
	}
	else
		m_position = destination;

	//Clamp the angle between 0 and 360 (prevents overflow and gives me nice numbers).
	m_angle += 360.0f;
	m_angle = fmod(m_angle, 360.0f);
}

float CPlayer::GetRadius()
{
	return m_radius;
}
