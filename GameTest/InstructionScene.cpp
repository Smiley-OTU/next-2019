#include "stdafx.h"
#include "InstructionScene.h"
#include "App/app.h"

CInstructionScene::CInstructionScene()
{
}

CInstructionScene::~CInstructionScene()
{
}

void CInstructionScene::Update(float deltaTime)
{
	const CController& controller = CSimpleControllers::GetInstance().GetController();
	if(controller.CheckButton(XINPUT_GAMEPAD_B))
		CScene::Change(ESceneType::MENU);
}

void CInstructionScene::Render()
{	//Green.
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	App::Print(50.0f, 500.0f, "Use the d-pad / WASD to move. Avoid those ghosts, they're spooky and can travel through walls due to 'features'.");
	App::Print(300.0f, 400.0f, "Press Escape / the B button to return to the main menu.");
}
