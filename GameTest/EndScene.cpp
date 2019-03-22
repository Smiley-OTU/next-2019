#include "stdafx.h"
#include "EndScene.h"
#include "../glut/include/GL/freeglut.h"

CEndScene::CEndScene()
{
}

CEndScene::~CEndScene()
{
}

void CEndScene::Update(float deltaTime)
{
}

void CEndScene::Render()
{	//Purple.
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
