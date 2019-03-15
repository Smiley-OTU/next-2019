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
	march(map, viewer.m_position, Math::direction(viewer.m_angle));
	//Ensure the rotation is between 0 and 360 so the quadrant check works.
	//float clampedAngle = fmodf(viewer.m_angle, 360.0f);
	//if (clampedAngle < 0.0f)
	//	clampedAngle += 360.0f;
	const float angleStep = viewer.m_fov / (float)m_count;
	const float raysStart = /*clampedAngle*/viewer.m_angle - viewer.m_fov * 0.5f;
	//Another way to do this would be viewer.m_angle + halfFov * indexToStep(i); The current way is more straight forward and less expensive. 
	for (size_t i = 0; i < m_count; i++) {
		//Angle of the ray, relative to the world origin (calculated based on the viewer's angle).
		float rayAngle = raysStart + angleStep * (float)i;
		//bool up = rayAngle < 180.0f;
		//bool right = rayAngle -> something about less than 90 but greater than 270 (impossible).
		//In conclusion, fuck this stupid janky marching algorithm.
		//Its not performant, elegant, nor personal!
		//I'm gonna extend rays till they hit the edge of the map, then step (using the known point algorithm) till I pick up a wall tile.
		//Get some visuals going on in a separate function maybe.

		//2D render:
		//CPoint rayDirection{ Math::direction(rayAngle) };
		//CLine ray{ viewer.m_position.x, viewer.m_position.y, viewer.m_position.x + rayDirection.x * 100.0f, viewer.m_position.y + rayDirection.y * 100.0f };
		//App::DrawLine(ray);
	}

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

	//float xRemainder = (int)position.x % (int)tileWidth;
	float xRemainder = fmodf(position.x, tileWidth);
	float xEdge = direction.x >= 0.0f ? position.x + tileWidth - xRemainder : position.x - xRemainder;
	float xDistance = xEdge - position.x;
	float unitRun = direction.x / direction.y;//if y = 1, x = 1 * unitRun.
	float xRate = xDistance / unitRun;
	//x2 +y2 = 1.
	//x = sqrt(1 - y2)
	float unitRun2 = sqrt(1.0f - Math::radians(direction.y) * Math::radians(direction.y));
	float xRate2 = xDistance / unitRun2;

	//float yRemainder = (int)position.y % (int)tileHeight;
	float yRemainder = fmodf(position.y, tileHeight);
	float yEdge = direction.y >= 0.0f ? position.y + tileHeight - yRemainder : position.y - yRemainder;
	float yDistance = yEdge - position.y;
	float unitRise = direction.y / direction.x;//if x = 1, y = 1 * unitRise.
	float yRate = yDistance / unitRise;
	float unitRise2 = sqrt(1.0f - Math::radians(direction.x) * Math::radians(direction.x));
	float yRate2 = yDistance / unitRise2;

	CPoint poi;
	if (abs(yRate2) < abs(xRate2)) {
		poi.y = position.y + yDistance;
		poi.x = position.x + yDistance * unitRun;
	}
	else {
		poi.x = position.x + xDistance;
		poi.y = position.y + xDistance * unitRise;
	}
	/*if (abs(yRate) < abs(xRate)) {
		poi.y = position.y + yDistance;
		poi.x = position.x + yDistance * unitRun;
	}
	else {
		poi.x = position.x + xDistance;
		poi.y = position.y + xDistance * unitRise;
	}*/
	//Remember to start from poi next iteration.
	//Also, we only need to check the direction once rather than every iteration cause it doesn't change between tiles.
	//Could try changing up the way the calculation is done but for now the branch condition needs to stay.
	
	static bool print = true;
	if (print) {
		//Tiles are 64x48.
		//x remainder : 52.000000, x edge : 512.000000, x step : 12.000000.
		//y remainder : 10.000000, y edge : 288.000000, y step : 38.000000.
		Math::print(poi, "Poi:\t\t");
		printf("x remainder: %f, x edge: %f, x distance: %f, unit run:  %f, x rate: %f.\n", xRemainder, xEdge, xDistance, unitRun, xRate);
		printf("y remainder: %f, y edge: %f, y distance: %f, unit rise: %f, y rate: %f.\n", yRemainder, yEdge, yDistance, unitRise, yRate);
		printf("Unit run  %f Unit rate x2%f\n", unitRun2, xRate2);
		printf("Unit rise %f Unit rate y2%f\n", unitRise2, yRate2);
		print = false;
	}

	App::DrawLine(position, CPoint{ direction.x * 500.0f, direction.y * 500.0f });
	App::DrawPoint(position, 5.0f);
	App::DrawPoint(poi, 5.0f, 0.0f, 1.0f, 0.0f);
}
