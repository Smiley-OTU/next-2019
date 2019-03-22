#pragma once
#include "Scene.h"
class CMenuScene :
	public CScene
{
public:
	CMenuScene();
	~CMenuScene();

	void Update(float deltaTime) override;
	void Render() override;

private:

};

