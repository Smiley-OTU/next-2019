#include "stdafx.h"
#include "MenuScene.h"
#include "App/app.h"

CMenuScene::CMenuScene()
{
}

CMenuScene::~CMenuScene()
{
}

void CMenuScene::Update(float deltaTime)
{
	const CController& controller = CSimpleControllers::GetInstance().GetController();
	//if (App::IsKeyPressed(Keys::Return))
	if(controller.CheckButton(XINPUT_GAMEPAD_A))
		CScene::Change(ESceneType::MAIN);

	//else if (App::IsKeyPressed(Keys::Backspace))
	else if(controller.CheckButton(XINPUT_GAMEPAD_Y))
		CScene::Change(ESceneType::INSTRUCTION);

	//if (App::IsKeyPressed(Keys::Escape))
	else if(controller.CheckButton(XINPUT_GAMEPAD_B))
		exit(0);
}

void CMenuScene::Render()
{	//Red.
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//I made this in 5 minutes. I realize the formmatting could be much better and I don't need a unique point for each x cause all 3 are the same.
	//What I had in mind was selectable buttons via the dpad. Figured I'd used the dpad rather than analog / mouse cause it would save time to emulate rather than support 2 input devices.
	static const float buttonWidth = 500.0f;
	static const float buttonHeight = 150.0f;
	static const float x = APP_VIRTUAL_WIDTH * 0.5f - buttonWidth * 0.5f;

	static const CPoint b1{ x, APP_VIRTUAL_HEIGHT * 0.75 };
	static const CPoint b2{ x, APP_VIRTUAL_HEIGHT * 0.50 };
	static const CPoint b3{ x, APP_VIRTUAL_HEIGHT * 0.25 };

	App::DrawQuad(b1.x, b1.y, b1.x + buttonWidth, b1.y + buttonHeight);
	App::DrawQuad(b2.x, b2.y, b2.x + buttonWidth, b2.y + buttonHeight);
	App::DrawQuad(b3.x, b3.y, b3.x + buttonWidth, b3.y + buttonHeight);

	App::Print(b1.x + 20.0f, b1.y + buttonHeight * 0.5f, "Press Enter / the A button to play!", 0.0f, 0.0f, 0.9f);
	App::Print(b2.x + 20.0f, b2.y + buttonHeight * 0.5f, "Press Backspace / the Y button for instructions.", 0.0f, 0.0f, 0.9f);
	App::Print(b3.x + 20.0f, b3.y + buttonHeight * 0.5f, "Press Escape / the B button to quit.", 0.0f, 0.0f, 0.9f);
}
