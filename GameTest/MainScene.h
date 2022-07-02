#pragma once
#include "Scene.h"
#include "SimpleTileMap.h"
#include "RayCaster.h"
#include "Player.h"
#include "Sprite.h"

class CMainScene :
	public CScene
{
public:
	CMainScene();
	~CMainScene();

	void Update(float deltaTime) override;
	void Render() override;

private:
	void OnEnter() override;
	void RenderMinimap();

	CSimpleTileMap m_map{ 16 };
	CRayCaster m_rayCaster{ 4.0f };
	CPlayer m_player;

	std::array<CSprite, 4> m_ghosts;
	const float m_actorRadius;
};