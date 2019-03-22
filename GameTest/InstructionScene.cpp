#include "stdafx.h"
#include "InstructionScene.h"
#include "../glut/include/GL/freeglut.h"

CInstructionScene::CInstructionScene()
{
}

CInstructionScene::~CInstructionScene()
{
}

void CInstructionScene::Update()
{
}

void CInstructionScene::Render()
{	//Green.
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
