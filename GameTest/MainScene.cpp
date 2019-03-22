#include "stdafx.h"
#include "MainScene.h"
#include "../glut/include/GL/freeglut.h"

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
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Fake 3D:
	m_rayCaster.Render(m_map, m_player);
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 300.0f, 300.0f });
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 400.0f, 300.0f });
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 500.0f, 300.0f });
	m_rayCaster.RenderSprite(m_map, m_player, CPoint{ 600.0f, 300.0f });
	m_rayCaster.clearDepthBuffer();
}

void CMainScene::OnEnter()
{
	m_map.RandomMap(80, 12);
	m_player.setFov(60.0f);
	m_player.setPosition(390.0f, 431.0f);
	m_player.setDirection(90.0f);
}
