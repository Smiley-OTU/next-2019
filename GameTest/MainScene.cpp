#include "stdafx.h"
#include "MainScene.h"
#include "App/app.h"

#define BLINKY 0
#define PINKY 1
#define INKY 2
#define CLYDE 3
#define NUM_GHOSTS 4

CMainScene::CMainScene()
{
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
	m_player.Update(m_map, deltaTime);
	m_rayCaster.Update(m_map, m_player);
}

void CMainScene::Render()
{
	const float halfHeight = APP_VIRTUAL_HEIGHT * 0.5f;
	App::DrawQuad(0.0f, 0.0f, APP_VIRTUAL_WIDTH, halfHeight, 0.2f, 0.2f, 0.2f);					//Floor
	App::DrawQuad(0.0f, halfHeight, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, 0.3f, 0.3f, 0.3f);	//Ceiling

	//Map
	m_rayCaster.RenderMap(m_map, m_player);

	//Sprites (depth sorted explicitly due to last minute improvements). A better implementation would only sort stuff that's queued for rendering.
	std::map<float, CSprite*> depthSortedSprites;
	for (int i = 0; i < NUM_GHOSTS; i++) {
		float distance = Math::l1norm(CPoint{ m_player.GetPosition() - m_ghosts[i].position });
		//Although sprites are small in size, its unnecessary to copy them so I used weak pointers. References aren't applicable in this case.
		depthSortedSprites[distance] = &m_ghosts[i];
	}
	for(auto it = depthSortedSprites.rbegin(); it != depthSortedSprites.rend(); ++it)
		m_rayCaster.RenderSprite(m_map, m_player, *it->second);

	RenderMinimap();

	m_rayCaster.clearDepthBuffer();
}

void CMainScene::OnEnter()
{
	m_map.RandomMap(80, 12);
	m_player.SetFov(45.0f);
	m_player.SetPosition(390.0f, 431.0f);
	m_player.SetDirection(90.0f);

	m_ghosts[BLINKY].position = CPoint{ 200.0f, 300.0f };
	m_ghosts[PINKY].position = CPoint{ 300.0f, 300.0f };
	m_ghosts[INKY].position = CPoint{ 400.0f, 300.0f };
	m_ghosts[CLYDE].position = CPoint{ 500.0f, 300.0f };
	
	//Replace hard coding with a function that seeks out non-wall tiles in an area otherwise we risk placing a ghost in a wall.
	//for (int i = 0; i < NUM_GHOSTS; i++) {
	//	m_ghosts[i].findPosition();
	//}
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
	App::DrawPoint(playerPosition, m_player.GetRadius(), 0.8f, 0.8f, 0.0f);
	glLineWidth(1.0f);
	App::DrawLine(playerPosition.x, playerPosition.y, playerPosition.x + playerDirection.x * 50.0f, playerPosition.y + playerDirection.y * 50.0f, 0.8f, 0.8f, 0.0f);
	glViewport(0.0f, 0.0f, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
}