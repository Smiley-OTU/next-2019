#pragma once
#include "Scene.h"
class CMenuScene :
	public CScene
{
public:
	CMenuScene();
	~CMenuScene();

	void Update() override;
	void Render() override;

private:

};

