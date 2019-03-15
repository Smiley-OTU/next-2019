#include "stdafx.h"
#include "RayCaster.h"
#include "App/app.h"
#include "SimpleTileMap.h"
#include "Viewer.h"
#include "Line.h"

CRayCaster::CRayCaster(float thickness) :
	m_count(APP_VIRTUAL_WIDTH / (size_t)thickness), m_thickness(thickness), m_step((float)m_count * thickness), m_rayOriginY(APP_VIRTUAL_HEIGHT / 2.0f)
{	//Make sure thickness is between 1 and 32.
	assert(thickness >= 1.0f && thickness <= 31.0f);
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

void CRayCaster::Render(const CSimpleTileMap& map, const CViewer& viewer)//Ensure the viewer's angle never exceeds 360.
{
	//Consider clamping the angle between 0 and 360, or -360 and 360. Can use Math::map() or %. 
	const float angleStep = viewer.m_fov / (float)m_count;
	const float raysStart = viewer.m_angle - viewer.m_fov * 0.5f;
	//Another way to do this would be viewer.m_angle + halfFov * indexToStep(i); The current way is more straight forward and less expensive. 
	for (size_t i = 0; i < m_count; i++) {
		//Angle of the ray, relative to the world origin (calculated based on the viewer's angle).
		float rayAngle = raysStart + angleStep * (float)i;

		//2D render:
		CPoint rayDirection{ Math::direction(rayAngle) };
		CLine ray{ viewer.m_position.x, viewer.m_position.y, viewer.m_position.x + rayDirection.x * 1000.0f, viewer.m_position.y + rayDirection.y * 1000.0f };
		App::DrawLine(ray);
	}

	march(map, viewer.m_position, Math::direction(viewer.m_angle));
	//3D render:
	/*float x = 0.0f;
	glLineWidth(m_thickness);
	for (size_t i = 0; i < m_count; i++) {
		App::DrawLine(x, m_rayOriginY - m_heightBuffer[i], x, m_rayOriginY + m_heightBuffer[i], m_colourBuffer[i].r, m_colourBuffer[i].g, m_colourBuffer[i].b);
		x += m_thickness;
	}*/
}

inline float CRayCaster::indexToStep(size_t index)
{
	return Math::map(index, 0.0f, m_count, -1.0f, 1.0f);
}

inline size_t CRayCaster::stepToIndex(float step)
{
	return Math::map(step, -1.0f, 1.0f, 0.0, m_count);
}


inline void CRayCaster::march(const CSimpleTileMap & map, const CPoint& position, const CPoint& direction)
{	//Returns out of bounds if outside the border -> store previous value, if we hit oob, assign to previous value (only case is air, so maybe just direct assign, no save)
	EMapValue result = EMapValue::OUTOFBOUNDS;
	float tileWidth = map.getTileWidth();
	float tileHeight = map.getTileHeight();

	float xRemainder = fmodf(position.x, tileWidth);
	float xEdge = direction.x >= 0.0f ? position.x + tileWidth - xRemainder : position.x - xRemainder;
	float xDistance = xEdge - position.x;
	float unitRun = direction.x / direction.y;//if y = 1, x = 1 * unitRun.
	float xRate = xDistance / unitRun;
	float xRate2 = xDistance / direction.x;

	float yRemainder = fmodf(position.y, tileHeight);
	float yEdge = direction.y >= 0.0f ? position.y + tileHeight - yRemainder : position.y - yRemainder;
	float yDistance = yEdge - position.y;
	float unitRise = direction.y / direction.x;//if x = 1, y = 1 * unitRise.
	float yRate = yDistance / unitRise;
	float yRate2 = yDistance / direction.y;

	CPoint poi;
	if (abs(yRate2) < abs(xRate2)) {
		poi.y = position.y + yDistance;
		poi.x = position.x + yDistance * unitRun;
	}
	else {
		poi.x = position.x + xDistance;
		poi.y = position.y + xDistance * unitRise;
	}
	//Remember to start from poi next iteration.
	//Also, we only need to check the direction once rather than every iteration cause it doesn't change between tiles.
	//Could try changing up the way the calculation is done but for now the branch condition needs to stay.
	
	static bool print = true;
	if (print) {
		printf("Unit run:  %f, x rate: %f.\n", unitRun, xRate);
		printf("Unit rise: %f, y rate: %f.\n", unitRise, yRate);
		printf("Direction x: %f, direction y: %f.\n", direction.x, direction.y);
		print = false;
	}

	App::DrawLine(position, CPoint{ position.x + direction.x * 500.0f, position.y + direction.y * 500.0f }, 1.0f, 0.0f, 0.0f);
	App::DrawPoint(position, 5.0f);
	App::DrawPoint(poi, 5.0f, 0.0f, 1.0f, 0.0f);
}
