#include "stdafx.h"
#include "Player.h"
#include <Windows.h>
#include "Keycodes.h"

CPlayer::CPlayer() : m_translationSpeed(1.0f), m_rotationSpeed(50.0f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(float deltaTime)
{
	deltaTime /= 1000.0f;
	float translationVelocity = m_translationSpeed * deltaTime;
	float rotationVelocity = m_rotationSpeed * deltaTime;

	if (GetAsyncKeyState(Keys::A))
		m_angle -= rotationVelocity;
	else if (GetAsyncKeyState(Keys::D))
		m_angle += rotationVelocity;

}