#pragma once
#include "Entity.h"

class CRayCaster;
class CViewer :
	public CEntity
{	//RayCaster uses this a lot so its nice to not have to call accessors.
	friend CRayCaster;
public:
	CViewer();
	~CViewer();

	//No need for this outside of RayCaster. RayCaster's implementation is optimized cause the distance calculation can be re-used.
	//bool inFov(const CPoint& point);

	//Returns the field of view in degrees.
	float GetFov();
	
	//Sets the field of view in degrees.
	void SetFov(float fov);

private:
	//Horizontal field of view expressed as an angle in degrees.
	float m_fov;
};

