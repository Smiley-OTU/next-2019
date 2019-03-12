#include "stdafx.h"
#include "RayCaster.h"
#include "App/app.h"
#include "Viewer.h"
#include "Line.h"
#define DRAW_2D true

CRayCaster::CRayCaster(float thickness) :
	m_count(APP_VIRTUAL_WIDTH / (size_t)thickness), m_thickness(thickness), m_step((float)m_count * thickness), m_rayOriginY(APP_VIRTUAL_HEIGHT / 2.0f)
{	//Make sure thickness is between 1 and 32.
	assert(thickness > 0.0f && thickness <= 31.0f);
	m_colourBuffer.resize(m_count);
	m_heightBuffer.resize(m_count);
}

CRayCaster::~CRayCaster()
{
}

void CRayCaster::Update()
{	//Don't iterate with floats. Mapping from floats to indices may fail due to percision errors.
	for (size_t i = 0; i < m_count; i++) {
		//float c = indexToStep(i);
		//TODO: assign colour based on colour of poi tile.
		m_colourBuffer[i].Randomize();
		//TODO: assign height based on projected and real height of poi tile.
		m_heightBuffer[i] = FRAND_RANGE(0.0f, APP_VIRTUAL_HEIGHT / 2.0f);
	}
}

void CRayCaster::Render(const CSimpleTileMap& map, const CViewer& viewer)
{
#if DRAW_2D
	//Placeholder till we can intersect.
	const static float rayDistance = 100.0f;
	const float angleStep = viewer.m_fov / (float)m_count;
	const float raysStart = viewer.m_angle - viewer.m_fov * 0.5f;
	//Another way to do this would be viewer.m_angle + halfFov * indexToStep(i); The current way is more straight forward and less expensive. 
	for (size_t i = 0; i < m_count; i++) {
		//Angle of the ray, relative to the world origin (calculated based on the viewer's angle).
		float rayAngle = raysStart + angleStep * (float)i;
		CPoint rayDirection{ Math::direction(rayAngle) };
		CLine ray{ viewer.m_position.x, viewer.m_position.y, viewer.m_position.x + rayDirection.x * rayDistance, viewer.m_position.y + rayDirection.y * rayDistance };
		App::DrawLine(ray);
	}
#else
	float x = 0.0f;
	glLineWidth(m_thickness);
	for (size_t i = 0; i < m_count; i++) {
		float c = Math::bias(indexToStep(i));
		App::DrawLine(x, m_rayOriginY - m_heightBuffer[i], x, m_rayOriginY + m_heightBuffer[i], m_colourBuffer[i].r, m_colourBuffer[i].g, m_colourBuffer[i].b);
		//Increment x after render so we start at 0.0.
		x = c * m_step;
	}

	//The actual algorithm will do dy over dx based on ray direction, not directly from step.
	//What will happen is rayAngle = playerAngle + step, then do cos and sin for direction.
#endif
}

inline float CRayCaster::indexToStep(size_t index)
{
	return Math::map(index, 0.0f, m_count, -1.0f, 1.0f);
}

inline size_t CRayCaster::stepToIndex(float step)
{
	return Math::map(step, -1.0f, 1.0f, 0.0, m_count);
}
