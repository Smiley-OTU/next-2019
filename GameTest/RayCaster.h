#pragma once
class CRayCaster
{
public:
	CRayCaster(float, float, float = 1.0f);
	~CRayCaster();

	void Render();

	//Vertical position of rays.
	float rayOriginY;

private:
	const float m_start, m_range, m_step, m_thickness;
};

