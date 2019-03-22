#pragma once
#include "Scene.h"
class CInstructionScene :
	public CScene
{
public:
	CInstructionScene();
	~CInstructionScene();

	void Update(float deltaTime) override;
	void Render() override;

private:

};