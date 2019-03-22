#pragma once
#include "Scene.h"
class CEndScene :
	public CScene
{
public:
	CEndScene();
	~CEndScene();

	void Update() override;
	void Render() override;

private:

};

