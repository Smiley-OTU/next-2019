#include "stdafx.h"
#include "MenuScene.h"
#include "../glut/include/GL/freeglut.h"

CMenuScene::CMenuScene()
{
}

CMenuScene::~CMenuScene()
{
}

void CMenuScene::Update(float deltaTime)
{
}

void CMenuScene::Render()
{	//Red.
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
