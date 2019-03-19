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
private:
	float m_rotationSpeed;
	float m_translationSpeed;
	CPoint m_meme;
};