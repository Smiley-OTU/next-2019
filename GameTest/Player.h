#pragma once
#include "Viewer.h"
class CPlayer :
	public CViewer
{
public:
	CPlayer();
	~CPlayer();

	void Update(float deltaTime);
private:
	float m_rotationSpeed;
	float m_translationSpeed;
};