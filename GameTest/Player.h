#pragma once
#include "Viewer.h"
class CSimpleTileMap;
class CPlayer :
	public CViewer
{
public:
	CPlayer();
	~CPlayer();

	void Update(const CSimpleTileMap& map, float deltaTime);
	float GetRadius();

private:
	float m_rotationSpeed;
	float m_translationSpeed;
	float m_radius = 10.0f;
};