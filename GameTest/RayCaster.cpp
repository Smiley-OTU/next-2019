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
{
	//1. Find the nearest point of intersection with the edges of the map.
	CLine ray{ position, direction * 500.0f };
	glLineWidth(15.0f);
	App::DrawLine(ray, 0.0f, 1.0f, 0.0f);
	CPoint poi;
	float shortestDistance = 9001.0f;//Over 9000.
	for (unsigned char i = 0; i < map.s_numBorders; i++) {
		CPoint result;
		if (Math::intersect(ray, map.borders[i], result)) {
			float distance = Math::l1norm(CLine{ position, result });
			if (distance < shortestDistance) {
				shortestDistance = distance;
				poi = result;
			}
		}
	}
	//Correct the ray to end at the poi rather than *infinity*.
	ray.p2 = poi;
	glLineWidth(5.0f);
	App::DrawLine(ray, 1.0f, 0.0f, 0.0f);

	//2. Step through tiles that ray passes through to find the nearest non-air tile.
	float deltaX = ray.p2x - ray.p1x;
	float deltaY = ray.p2y - ray.p1y;
	float distance = Math::l2norm(deltaX, deltaY);
	float dx = deltaX / distance;
	float dy = deltaY / distance;
	//These are small because we're currently stepping per-pixel (not okay) rather than per-index.
	//printf("%f %f\n", dx, dy);
	
	for (int i = 0; i <= ceil(distance); i++) {
		float x = floor(ray.p1x + dx * i);
		float y = floor(ray.p1y + dy * i);
		//printf("%f %f\n", x, y);
	}
	printf("");
}
