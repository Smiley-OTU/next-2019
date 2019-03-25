#include "stdafx.h"
#include "Scene.h"
#include "MenuScene.h"
#include "InstructionScene.h"
#include "MainScene.h"
#include "EndScene.h"

CScene* CScene::s_scenes[NUM_SCENES];
CScene* CScene::s_activeScene = nullptr;
bool CScene::s_initialized = false;

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Change(ESceneType sceneType)
{
#if _DEBUG
	assert(s_initialized);
	assert(sceneType < ESceneType::NUM_SCENES);
#endif
	s_activeScene->OnExit();
	s_activeScene = s_scenes[sceneType];
	s_activeScene->OnEnter();
}

void CScene::Init()
{
	s_scenes[MENU] = new CMenuScene();
	s_scenes[INSTRUCTION] = new CInstructionScene();
	s_scenes[MAIN] = new CMainScene();
	s_scenes[END] = new CEndScene();

	s_initialized = true;
	s_activeScene = s_scenes[MENU];
	s_activeScene->OnEnter();
}

void CScene::UpdateActiveScene(float deltaTime)
{
	s_activeScene->Update(deltaTime);
}

void CScene::RenderActiveScene()
{
	s_activeScene->Render();
}

void CScene::Shutdown()
{
	delete s_scenes[MENU];
	delete s_scenes[INSTRUCTION];
	delete s_scenes[MAIN];
	delete s_scenes[END];
	s_initialized = false;
}

void CScene::OnEnter()
{
}

void CScene::OnExit()
{
}