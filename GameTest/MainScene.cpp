#include "stdafx.h"
#include "MainScene.h"
#include "../glut/include/GL/freeglut.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

void CMainScene::Update()
{
}

void CMainScene::Render()
{	//Blue.
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
