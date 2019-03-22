#pragma once
#include "Scene.h"
class CEndScene :
	public CScene
{
public:
	CEndScene();
	~CEndScene();

	void Update(float deltaTime) override;
	void Render() override;

private:

};

