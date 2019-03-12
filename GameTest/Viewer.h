#pragma once
#include "Entity.h"

class CRayCaster;
class CViewer :
	public CEntity
{
	friend CRayCaster;
public:
	CViewer();
	~CViewer();
	
	//Takes in the desired field of view as an angle in degrees. (Made a setter so fov can be dynamic).
	void setFov(float);

private:
	//Half the horizontal field of view expressed as an angle in degrees.
	//float m_halfFov;
	//Horizontal field of view expressed as an angle in degrees.
	float m_fov;
};

