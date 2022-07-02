#include "stdafx.h"
#include "MainScene.h"
#include "Pathing.h"
#include "App/app.h"

#define BLINKY 0
#define PINKY 1
#define INKY 2
#define CLYDE 3

CMainScene::CMainScene() : m_actorRadius(50.0f)
{
	m_map.RandomMap(80, 12);

	const float spriteWidth = 40.0f;
	const float spriteHeight = 30.0f;
	m_ghosts[BLINKY] = CSprite{ spriteWidth, spriteHeight, 1.0f, 0.0f, 0.0f };
	m_ghosts[PINKY] = CSprite{ spriteWidth, spriteHeight, 1.0f, 0.5f, 0.0f };
	m_ghosts[INKY] = CSprite{ spriteWidth, spriteHeight, 0.0f, 1.0f, 1.0f };
	m_ghosts[CLYDE] = CSprite{ spriteWidth, spriteHeight, 1.0f, 0.5f, 1.0f };
}

CMainScene::~CMainScene()
{
}

void CMainScene::Update(float deltaTime)
{
	/*m_player.Update(m_map, deltaTime);
	m_rayCaster.Update(m_map, m_player);
	float ghostSpeed = m_player.GetSpeed() * 0.5f * deltaTime / 1000.0f;
	for (CSprite& ghost : m_ghosts) {
		CPoint toPlayer{ Math::normalize(m_player.GetPosition() - ghost.position) };
		ghost.position = Pathing::Ricochet(m_map, ghost.position, toPlayer * ghostSpeed);
		if (Math::circleCollision(m_player.GetPosition(), ghost.position, m_actorRadius * 0.25f, m_actorRadius * 0.5f))
			CScene::Change(ESceneType::END);
	}*/
}

void CMainScene::Render()
{
	/*const float halfHeight = APP_VIRTUAL_HEIGHT * 0.5f;
	App::DrawQuad(0.0f, 0.0f, APP_VIRTUAL_WIDTH, halfHeight, 0.2f, 0.2f, 0.2f);					//Floor
	App::DrawQuad(0.0f, halfHeight, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, 0.3f, 0.3f, 0.3f);	//Ceiling

	m_rayCaster.RenderMap(m_map, m_player);
	m_rayCaster.RenderSprites(m_map, m_player, m_ghosts);
	RenderMinimap();

	m_rayCaster.clearDepthBuffer();*/
	m_map.Render();

	using namespace Pathing;
	for (const CSprite& ghost : m_ghosts)
	{
		DrawPath(FindPath(
			m_map.GetCell(ghost.position),
			m_map.GetCell(m_player.GetPosition()),
			m_map
		), m_map);
	}
}

void CMainScene::OnEnter()
{
	m_player.SetPosition({ APP_VIRTUAL_WIDTH * 0.5f + m_map.GetTileWidth() * 0.5f, APP_VIRTUAL_HEIGHT * 0.5f + m_map.GetTileHeight() * 0.5f });
	m_player.SetDirection(90.0f);
	m_player.SetFov(75.0f);

	m_ghosts[BLINKY].position = CPoint{ float(m_map.GetTileWidth()) * 1.5f, float(m_map.GetTileHeight()) * 1.5f };
	m_ghosts[PINKY].position = CPoint{ float(m_map.GetTileWidth()) * 1.5f, float(APP_VIRTUAL_HEIGHT) - float(m_map.GetTileHeight()) * 3.5f };
	m_ghosts[INKY].position = CPoint{ float(APP_VIRTUAL_WIDTH) - float(m_map.GetTileWidth()) * 1.5f, float(APP_VIRTUAL_HEIGHT) - float(m_map.GetTileHeight()) * 1.5f };
	m_ghosts[CLYDE].position = CPoint{ float(APP_VIRTUAL_WIDTH) - float(m_map.GetTileWidth()) * 3.5f, float(m_map.GetTileHeight()) * 3.5f };
}

void CMainScene::RenderMinimap()
{
	static const float mapScale = 0.2f;		//20%  screen size minimap.
	static const float margin = 0.025f;		//2.5% screen size margin.
	static const float scaledScreenWidth = APP_VIRTUAL_WIDTH * mapScale;
	static const float scaledScreenHeight = APP_VIRTUAL_HEIGHT * mapScale;
	static const float x = (APP_VIRTUAL_WIDTH - scaledScreenWidth) - APP_VIRTUAL_WIDTH * margin;
	static const float y = APP_VIRTUAL_HEIGHT * margin;
	glViewport(x, y, scaledScreenWidth, scaledScreenHeight);
	
	m_map.Render();

	const CPoint playerPosition{ m_player.GetPosition() };
	const CPoint playerDirection{ m_player.GetDirection() };
	App::DrawPoint(playerPosition, m_actorRadius, 0.8f, 0.8f, 0.0f);
	glLineWidth(1.0f);
	App::DrawLine(playerPosition.x, playerPosition.y, playerPosition.x + playerDirection.x * 50.0f, playerPosition.y + playerDirection.y * 50.0f, 0.8f, 0.8f, 0.0f);

	for (const CSprite& ghost : m_ghosts)
		App::DrawPoint(ghost.position, m_actorRadius, ghost.r, ghost.g, ghost.b);

	glViewport(0.0f, 0.0f, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
}