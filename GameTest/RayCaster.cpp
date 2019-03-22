#include "stdafx.h"
#include "RayCaster.h"
#include "App/app.h"
#include "Viewer.h"
#include "Line.h"

//Increase distance till point of intersection by 5% so that the ray is guaranteed to exceed its current cell.
#define DISTANCE_MULTIPLIER 1.05f
#define MAX_STEPS 32
#define DRAW_2D true

CRayCaster::CRayCaster(float thickness) :
	m_count(APP_VIRTUAL_WIDTH / (uint32_t)thickness), m_thickness(thickness), m_step((float)m_count * thickness), m_rayOriginY(APP_VIRTUAL_HEIGHT * 0.5f)
{	//Make sure thickness is between 1 and 32.
	assert(thickness >= 1.0f && thickness <= 31.0f);
	m_indexBuffer.resize(m_count);
	m_poiBuffer.resize(m_count);
	m_depthBuffer.resize(m_count);
	clearDepthBuffer();
}

CRayCaster::~CRayCaster()
{
}

void CRayCaster::Update(const CSimpleTileMap& map, const CViewer& viewer)
{
	//Length of adjacent side of right triangle formed by the screen and the field of view.
	m_projectionDistance = abs((APP_VIRTUAL_WIDTH * 0.5f) / tanf(Math::radians(viewer.m_fov * 0.5f)));

	const float angleStep = viewer.m_fov / (float)m_count;
	const float raysStart = viewer.m_angle - viewer.m_fov * 0.5f;
	for (uint32_t i = 0; i < m_count; i++) {
		const float rayAngle = raysStart + angleStep * (float)i;
		const CPoint poi = march(map, viewer.m_position, Math::direction(rayAngle));
		m_indexBuffer[i] = map.GetTileMapValue(poi.x, poi.y);

		const float depth = Math::l2norm(poi - viewer.m_position) * cosf(Math::radians(viewer.m_angle - rayAngle));
		//glDepthFunc(GL_LESS) ;)
		if (depth < m_depthBuffer[i])
			m_depthBuffer[i] = depth;

#if DRAW_2D
		//Store debug information.
		m_poiBuffer[i] = poi;
#endif
	}
}

void CRayCaster::Render(const CSimpleTileMap& map, const CViewer& viewer)
{
#if DRAW_2D
	glViewport(APP_VIRTUAL_WIDTH * 0.5f, 0.0f, APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT);
	map.Render();
	for (uint32_t i = 0; i < m_count; i++)
		App::DrawLine(viewer.m_position.x, viewer.m_position.y, m_poiBuffer[i].x, m_poiBuffer[i].y);
	glViewport(0.0f, 0.0f, APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT);
#endif

	float x = 0.0f;
	glLineWidth(m_thickness);
	for (uint32_t i = 0; i < m_count; i++) {
		//Read based on index found in Update() rather than read copied data.
		const CTile& tile = CTile::tiles[m_indexBuffer[i]];
		//Projected height = actual height / distance to poi * distance to projection plane.
		const float projectedHeight = (tile.height / m_depthBuffer[i]) * m_projectionDistance;
		//Fake some 3D per verticle slice!
		App::DrawLine(x, m_rayOriginY - projectedHeight, x, m_rayOriginY + projectedHeight, tile.r, tile.g, tile.b);
		x += m_thickness;
	}
}

void CRayCaster::RenderSprite(const CSimpleTileMap& map, const CViewer& viewer, const CPoint& spritePosition)
{
	//Frustum culling:
	CPoint toSprite{ spritePosition - viewer.m_position };
	float toSpriteDistance = Math::l2norm(toSprite);
	toSprite /= toSpriteDistance;

	//Return if the sprite isn't within the viewer's field of view.
	if (Math::dot(toSprite, Math::direction(viewer.m_angle)) <= cosf(Math::radians(viewer.m_fov) * 0.5f))
		return;

	//Occlusion culling:
	CPoint poi = march(map, viewer.m_position, toSprite);
	float poiDistance = Math::l2norm(poi - viewer.m_position);

	//Return if the sprite is behind a wall.
	if (poiDistance < toSpriteDistance)
		return;

	//Angle between view direction and sprite.
	float spriteAngle = Math::radians(viewer.m_angle - Math::angle(toSprite));
	//Scale the sprite based on distance from the viewer.
	float scale = m_projectionDistance / abs(cosf(spriteAngle) * toSpriteDistance);
	//Figure out where x lies based on the sprite angle and the projection plane triangle.
	float x = tanf(spriteAngle) * m_projectionDistance;

	//Eventually pass in sprite width and height here. Be sure to do x - width / 2. Also render furthest sprites first.
	App::DrawPoint(APP_VIRTUAL_WIDTH * 0.5f + x, m_rayOriginY, scale * 40.0f, 1.0f, 0.0f, 0.0f);

#if DRAW_2D
	glViewport(APP_VIRTUAL_WIDTH * 0.5f, 0.0f, APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT);
	App::DrawLine(viewer.m_position, spritePosition, 1.0f, 0.0f, 0.0f);
	App::DrawPoint(spritePosition, 40.0f, 1.0f, 0.0f, 0.0f);
	glViewport(0.0f, 0.0f, APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT);
#endif
}

void CRayCaster::clearDepthBuffer()
{	//Over 9000 to ensure nothing will have a greater depth level.
	m_depthBuffer.assign(m_depthBuffer.size(), 9001.0f);
}

inline CPoint CRayCaster::march(const CSimpleTileMap& map, const CPoint& position, const CPoint& direction)
{
	const float tileWidth = map.getTileWidth();
	const float tileHeight = map.getTileHeight();

	const float unitRise = direction.y / direction.x;
	const float unitRun = direction.x / direction.y;

	//Continue searching until we find anything but a floor (air) or exceed the allowed amount of steps.
	EMapValue tileValue = EMapValue::FLOOR;
	uint32_t stepCount = 0;
	CPoint poi = position;
	while (tileValue == EMapValue::FLOOR) {
		const float xRemainder = fmodf(poi.x, tileWidth);
		const float xEdge = direction.x >= 0.0f ? poi.x + tileWidth - xRemainder : poi.x - xRemainder;
		const float xDistance = xEdge - poi.x;
		const float xRate = xDistance / direction.x;

		const float yRemainder = fmodf(poi.y, tileHeight);
		const float yEdge = direction.y >= 0.0f ? poi.y + tileHeight - yRemainder : poi.y - yRemainder;
		const float yDistance = yEdge - poi.y;
		const float yRate = yDistance / direction.y;

		//Increase the poi by a small percentage in order to ensure its in a new cell.
		//Move x proportional to how we moved y or vice versa based on nearest edge.
		if (abs(yRate) < abs(xRate)) {
			const float distance = yDistance * DISTANCE_MULTIPLIER;
			poi.y = poi.y + distance;
			poi.x = poi.x + distance * unitRun;
		}
		else {
			const float distance = xDistance * DISTANCE_MULTIPLIER;
			poi.x = poi.x + distance;
			poi.y = poi.y + distance * unitRise;
		}

		//Look up cell. Break out of loop and render border if we get some weird numbers.
		tileValue = map.GetTileMapValue(poi.x, poi.y);
		stepCount++;
		if (stepCount > MAX_STEPS) {
			tileValue = EMapValue::WALL;
			break;
		}
	}

	return poi;
}