#include "stdafx.h"
#include "RayCaster.h"
#include "App/app.h"
#include "SimpleTileMap.h"
#include "Viewer.h"
#include "Line.h"

//Increase distance till point of intersection by 10% so that the ray is guaranteed to exceed its current cell.
#define DISTANCE_MULTIPLIER 1.1f
#define MAX_STEPS 8

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

inline void CRayCaster::march(const CSimpleTileMap & map, const CPoint& position, const CPoint& direction)
{
	App::DrawLine(position, CPoint{ position.x + direction.x * 500.0f, position.y + direction.y * 500.0f }, 1.0f, 0.0f, 0.0f);
	float tileWidth = map.getTileWidth();
	float tileHeight = map.getTileHeight();

	float unitRise = direction.y / direction.x;
	float unitRun = direction.x / direction.y;

	CPoint poi = position;

	//Continue searching until we find anything but a floor (air) or go outside the map.
	EMapValue tileValue = EMapValue::FLOOR;
	while (tileValue == EMapValue::FLOOR) {
		float xRemainder = fmodf(poi.x, tileWidth);
		float xEdge = direction.x >= 0.0f ? poi.x + tileWidth - xRemainder : poi.x - xRemainder;
		float xDistance = xEdge - poi.x;
		float xRate = xDistance / direction.x;

		float yRemainder = fmodf(poi.y, tileHeight);
		float yEdge = direction.y >= 0.0f ? poi.y + tileHeight - yRemainder : poi.y - yRemainder;
		float yDistance = yEdge - poi.y;
		float yRate = yDistance / direction.y;

		if (abs(yRate) < abs(xRate)) {
			yDistance *= DISTANCE_MULTIPLIER;
			poi.y = poi.y + yDistance;
			poi.x = poi.x + yDistance * unitRun;
		}
		else {
			xDistance *= DISTANCE_MULTIPLIER;
			poi.x = poi.x + xDistance;
			poi.y = poi.y + xDistance * unitRise;
		}

		tileValue = map.GetTileMapValue(poi.x, poi.y);

		App::DrawPoint(poi, 15.0f, 1.0f, 0.0f, 0.0f);
	}

	//If we're exceeded the grid, render a wall in place of the border (this should never happen, should always hit a wall at the very least).
	if (tileValue = EMapValue::OUTOFBOUNDS)
		tileValue = EMapValue::WALL;
}