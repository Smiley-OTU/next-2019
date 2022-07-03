#pragma once
#include "Scene.h"
#include "SimpleTileMap.h"
#include "RayCaster.h"
#include "Player.h"
#include "Sprite.h"
#include <thread>

#define SINGLE_THREAD 0

#if !SINGLE_THREAD
#define THREAD_PERSISTENT 1
#if !THREAD_PERSISTENT
#define THREAD_RECREATE 1
#endif
#endif

class CMainScene :
	public CScene
{
public:
	CMainScene();
	~CMainScene();

	void Update(float deltaTime) override;
	void Render() override;

private:
	void OnEnter() override;
	void OnExit() override;
	void RenderMinimap();
	void UpdateGhosts(float deltaTime);

	CSimpleTileMap m_map{ 16 };
	CRayCaster m_rayCaster{ 4.0f };
	CPlayer m_player;

	Sprites m_ghosts;
	const float m_actorRadius;

#if !SINGLE_THREAD
	std::thread m_worker;
#endif

#if THREAD_PERSISTENT
	std::atomic_bool m_running = false;
	std::atomic_bool m_updatePaths = false;
	std::atomic<float> m_deltaTime = 0.0f;
#endif
};