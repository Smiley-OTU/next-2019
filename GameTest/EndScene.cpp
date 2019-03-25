#include "stdafx.h"
#include "EndScene.h"
#include "App/app.h"

CEndScene::CEndScene()
{
}

CEndScene::~CEndScene()
{
}

void CEndScene::Update(float deltaTime)
{
	const CController& controller = CSimpleControllers::GetInstance().GetController();
	//if (App::IsKeyPressed(Keys::Return))
	if (controller.CheckButton(XINPUT_GAMEPAD_A))
		CScene::Change(ESceneType::MENU);

	//if (App::IsKeyPressed(Keys::Escape))
	else if (controller.CheckButton(XINPUT_GAMEPAD_B))
		exit(0);
}

void CEndScene::Render()
{	//Purple.
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	App::Print(400.0f, 600.0f, "You got spooked :(");
	App::Print(400.0f, 500.0f, "Press Enter / the A button to play again");
	App::Print(400.0f, 400.0f, "Press Escape / the B button to quit.");
}
