#include "stdafx.h"
#include "MainScene.h"
#include "App/app.h"

CMainScene::CMainScene()
{
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
{	//Blue.
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0.0f, 0.0f, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);
	//Fake 3D:
	const float halfHeight = APP_VIRTUAL_HEIGHT * 0.5f;
	App::DrawQuad(0.0f, 0.0f, APP_VIRTUAL_WIDTH, halfHeight, 0.2f, 0.2f, 0.2f);					//Floor
	App::DrawQuad(0.0f, halfHeight, APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT, 0.3f, 0.3f, 0.3f);	//Ceiling

	m_rayCaster.Render(m_map, m_player);
	RenderMinimap();
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 200.0f, 300.0f }, 40.0f, 30.0f, 0.0f, 0.2f, 1.0f);
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 300.0f, 300.0f }, 40.0f, 30.0f, 1.0f, 0.0f, 0.0f);
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 400.0f, 300.0f }, 40.0f, 30.0f, 1.0f, 0.5f, 0.0f);
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 500.0f, 300.0f }, 40.0f, 30.0f, 1.0f, 0.0f, 1.0f);
	m_rayCaster.clearDepthBuffer();
}

void CMainScene::OnEnter()
{
	m_map.RandomMap(80, 12);
	m_player.SetFov(45.0f);
	m_player.SetPosition(390.0f, 431.0f);
	m_player.SetDirection(90.0f);
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
