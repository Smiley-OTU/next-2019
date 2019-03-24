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
	const float rotationVelocity = m_rotationSpeed * deltaTime;

	if(App::IsKeyPressed(Keys::A))
		m_angle -= rotationVelocity;
	else if (App::IsKeyPressed(Keys::D))
		m_angle += rotationVelocity;

	const float translationVelocity = m_translationSpeed * deltaTime;
	const CPoint direction{ Math::direction(m_angle) };
	CPoint translation{ direction * translationVelocity };
	if (App::IsKeyPressed(Keys::W))
		translation *= 1.0f;
	else if (App::IsKeyPressed(Keys::S))
		translation *= -1.0f;
	else
		translation *= 0.0f;

	//printf("px: %f py: %f dx: %f dy: %f ox: %f oy: %f.\n", m_position.x, m_position.y, destination.x, destination.y, Math::overlap(destination.x, direction.x, map.getTileWidth()), Math::overlap(destination.y, direction.y, map.getTileHeight()));
	CPoint destination{ m_position + translation };
	EMapValue destinationValue = map.GetTileMapValue(destination.x, destination.y);
	if (destinationValue == EMapValue::BORDER || destinationValue == EMapValue::WALL || destinationValue == EMapValue::OUTOFBOUNDS) {
		/*const float tileWidth = map.getTileWidth();
		const float tileHeight = map.getTileHeight();

		const int xIndex = (int)(destination.x / tileWidth);
		const int yIndex = (int)(destination.y / tileHeight);

		const float xEdge = xIndex * tileWidth;
		const float yEdge = yIndex * tileHeight;

		destination.x -= destination.x - xEdge;
		destination.y -= destination.y - yEdge;*/
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
