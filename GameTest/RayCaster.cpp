#include "stdafx.h"
#include "RayCaster.h"
#include "App/app.h"

CRayCaster::CRayCaster(float xMin, float xMax, float thickness) :
	m_start(xMin), m_range(xMax - xMin), m_step(thickness * 2.0f / m_range), m_thickness(thickness)
{	//Make sure thickness is between 1 and 32.
	assert(thickness > 0.0f && thickness <= 31.0f);
}

CRayCaster::~CRayCaster()
{
}

void CRayCaster::Render()
{	//Height is a placeholder for now. Once a map is in place, we will scale based on projected * real (based on distance).
	static float height = APP_VIRTUAL_HEIGHT / 3.0f;
	glLineWidth(m_thickness);
	for (float i = -1.0f; i < 1.0f; i += m_step) {
		float c = Math::bias(i);
		float x = m_start + c * m_range;
		App::DrawLine(x, rayOriginY - height, x, rayOriginY + height, c, 1.0f - c, FRAND_RANGE(0.0f, 1.0f));
	}
	
	//The actual algorithm will do dy over dx based on ray direction, not directly from step.
	//What will happen is rayAngle = playerAngle + step, then do cos and sin for direction.
}
