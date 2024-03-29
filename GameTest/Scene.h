#pragma once
#include <array>

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	static void Init();
	static void UpdateActiveScene(float deltaTime);
	static void RenderActiveScene();
	static void Shutdown();

protected:
	virtual void OnEnter();
	virtual void OnExit();

	enum ESceneType : uint32_t {
		MENU,
		INSTRUCTION,
		MAIN,
		END,
		NUM_SCENES
	};

	static void Change(ESceneType sceneType);

	static std::array<CScene*, NUM_SCENES> s_scenes;
	static CScene* s_activeScene;
	static bool s_initialized;
};