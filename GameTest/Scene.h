#pragma once

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual void Update() = 0;
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

	static CScene* s_scenes[NUM_SCENES];
	static CScene* s_activeScene;
	static bool s_initialized;
};

