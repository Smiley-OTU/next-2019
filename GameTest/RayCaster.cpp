#include "stdafx.h"
#include "RayCaster.h"
#include "App/app.h"

CRayCaster::CRayCaster(float xMin, float xMax, float thickness) :
	m_xMin(xMin), m_xMax(xMax), m_range(xMax - xMin), m_step((thickness * 2.0f) / (xMax - xMin)), m_thickness(thickness), m_count((size_t)m_range)
{	//Make sure thickness is between 1 and 32.
	assert(thickness > 0.0f && thickness <= 31.0f);
	m_colourBuffer.resize(m_range);
	m_heightBuffer.resize(m_range);
}

CRayCaster::~CRayCaster()
{
}

void CRayCaster::Update()
{
	/*for (size_t i = 0; i < (size_t)m_range; i++) {
		float c = Math::map((float)i, 0.0f, m_range, -1.0f, 1.0f);
		printf("%f\n", c);
	}*/
	float value = -1.0f;
	for (float i = -1.0f; i < 1.0f; i += m_step) {
		value += m_step;
		size_t x = Math::map(i, -1.0f, 1.0f, m_xMin, m_xMax);
		x -= m_xMin;
		x /= m_thickness;
		//Can optimize out the -outMax at the end cause we want to start from 0. Verify that the resultant start will always be 0 in Math::map.

		m_colourBuffer[x].Randomize();
		m_heightBuffer[x] = FRAND_RANGE(0.0f, APP_VIRTUAL_HEIGHT / 2.0f);
	}
}

void CRayCaster::Render()
{
	glLineWidth(m_thickness);
	for (float i = -1.0f; i < 1.0f; i += m_step) {
		float c = Math::bias(i) * m_range + m_xMin;
		size_t x = Math::map(i, -1.0f, 1.0f, m_xMin, m_xMax);
		x -= m_xMin;
		x /= m_thickness;
		App::DrawLine(c, rayOriginY - m_heightBuffer[x], c, rayOriginY + m_heightBuffer[x], m_colourBuffer[x].r, m_colourBuffer[x].g, m_colourBuffer[x].b);
	}
	
	//The actual algorithm will do dy over dx based on ray direction, not directly from step.
	//What will happen is rayAngle = playerAngle + step, then do cos and sin for direction.
}

inline float CRayCaster::mapToStep(size_t index)
{
	return 0.0f;
}

inline size_t CRayCaster::mapToIndex(float step)
{
	return size_t();
}
