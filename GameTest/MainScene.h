#pragma once
#include "Scene.h"
class CMainScene :
	public CScene
{
public:
	CMainScene();
	~CMainScene();

	void Update() override;
	void Render() override;

private:

};

