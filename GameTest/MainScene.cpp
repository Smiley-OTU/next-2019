#include "stdafx.h"
#include "MainScene.h"
#include "App/app.h"
#include "Pathing.h"

#define BLINKY 0
#define PINKY 1
#define INKY 2
#define CLYDE 3

CMainScene::CMainScene() : m_actorRadius(50.0f)
{
	const float spriteWidth = 40.0f;
	const float spriteHeight = 30.0f;
	m_ghosts.resize(4);
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
	m_player.Update(m_map, deltaTime);
	m_rayCaster.Update(m_map, m_player);
	float ghostSpeed = m_player.GetSpeed() * 0.5f *  deltaTime / 1000.0f;
	for (CSprite& ghost : m_ghosts) {
		CPoint toPlayer{ Math::normalize(m_player.GetPosition() - ghost.position) };
		ghost.position += toPlayer * ghostSpeed;
		//if (Math::circleCollision(m_player.GetPosition(), ghost.position, m_actorRadius * 0.25f, m_actorRadius * 0.5f))
		//	CScene::Change(ESceneType::END);
	}
}

void CMainScene::Render()
{
	const float halfHeight = APP_VIRTUAL_HEIGHT * 0.5f;
	App::DrawQuad(0.0f, 0.0f, APP_VIRTUAL_WIDTH, halfHeight, 0.2f, 0.2f, 0.2f);					//Floor
	App::DrawQuad(0.0f, halfHeight, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, 0.3f, 0.3f, 0.3f);	//Ceiling

	m_rayCaster.RenderMap(m_map, m_player);
	m_rayCaster.RenderSprites(m_map, m_player, m_ghosts);
	RenderMinimap();

	m_rayCaster.clearDepthBuffer();
}

void CMainScene::OnEnter()
{
	m_map.RandomMap(80, 12);

	//Place the player on air. If no luck, go back to main menu and try again rather than doing my convoluted algorithm.
	CPoint playerPosition{ APP_VIRTUAL_WIDTH * 0.5f + m_map.getTileWidth() * 0.5f, APP_VIRTUAL_HEIGHT * 0.5f + m_map.getTileHeight() * 0.5f };
	if(m_map.GetTileMapValue(playerPosition.x, playerPosition.y) != EMapValue::AIR)
		CScene::Change(ESceneType::MENU);
	/*
	Cell playerCell = m_map.GetCell(playerPosition);
	m_player.SetPosition(playerPosition);
	EMapValue playerTile = m_map.GetTileMapValue(playerPosition.x, playerPosition.y);
	int searchArea = 1;
	while (playerTile != EMapValue::AIR) {
		for (int i = -searchArea; i < searchArea; i++) {
			for (int j = -searchArea; j < searchArea; j++) {
				playerTile = m_map.GetTileMapValue(playerCell.first + i, playerCell.second + j);
				if (playerTile == AIR) {
					playerPosition = CPoint{ (playerCell.first + i) * m_map.getTileWidth() + m_map.getTileWidth() * 0.5f, (playerCell.second + j) * m_map.getTileHeight() + m_map.getTileHeight() * 0.5f };
					break;
				}
			}
		}
		searchArea++;
		//If this somehow fails, simply go back to the main menu as if nothing happened.
		if (searchArea >= m_map.GetMapSize() / 2)
			CScene::Change(ESceneType::MENU);
	}*/
	m_player.SetPosition(playerPosition);
	m_player.SetDirection(90.0f);
	m_player.SetFov(75.0f);

	m_ghosts[BLINKY].position = CPoint{ 100.0f, 100.0f };
	m_ghosts[PINKY].position = CPoint{ APP_VIRTUAL_WIDTH - 100.0f, 100.0f };
	m_ghosts[INKY].position = CPoint{ 100.0f, APP_VIRTUAL_HEIGHT - 100.0f };
	m_ghosts[CLYDE].position = CPoint{ APP_VIRTUAL_WIDTH - 100.0f, APP_VIRTUAL_HEIGHT - 100.0f };
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