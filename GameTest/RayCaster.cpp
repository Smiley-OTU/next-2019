#include "stdafx.h"
#include "RayCaster.h"
#include "App/app.h"
#include "Viewer.h"
#include "Line.h"

#define DEBUG_DRAW false

CRayCaster::CRayCaster(float thickness) :
	m_count(APP_VIRTUAL_WIDTH / (uint32_t)thickness), m_thickness(thickness), m_rayOriginY(APP_VIRTUAL_HEIGHT * 0.5f)
{	//Make sure thickness is between 1 and 32.
	assert(thickness >= 1.0f && thickness <= 31.0f);
	m_indexBuffer.resize(m_count);
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

		//Form depth buffer based off fisheye-corrected nearest surface. Used for debug rendering, would be more useful for multi-pass rendering.
		const float depth = Math::l2norm(poi - viewer.m_position) * cosf(Math::radians(viewer.m_angle - rayAngle));
		//glDepthFunc(GL_LESS) ;)
		if (depth < m_depthBuffer[i])
			m_depthBuffer[i] = depth;
	}
}

void CRayCaster::RenderMap(const CSimpleTileMap& map, const CViewer& viewer)
{
#if DEBUG_DRAW
	glViewport(APP_VIRTUAL_WIDTH * 0.5f, 0.0f, APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT);
	glLineWidth(1.0f);

	//Map.
	map.Render();

	//Rays.
	const float angleStep = viewer.m_fov / (float)m_count;
	const float raysStart = viewer.m_angle - viewer.m_fov * 0.5f;
	for (uint32_t i = 0; i < m_count; i++) {
		const CPoint direction{ Math::direction(raysStart + angleStep * (float)i) };
		App::DrawLine(viewer.m_position.x, viewer.m_position.y, viewer.m_position.x + direction.x * m_depthBuffer[i], viewer.m_position.y + direction.y * m_depthBuffer[i]);
	}

	//Vertical lines.
	const float tileWidth = map.getTileWidth();
	for (int x = tileWidth; x < map.GetMapSize() * tileWidth; x += tileWidth)
		App::DrawLine(x, 0.0f, x, APP_VIRTUAL_HEIGHT);

	//Horizontal lines.
	const float tileHeight = map.getTileHeight();
	for (int y = tileHeight; y < map.GetMapSize() * tileHeight; y += tileHeight)
		App::DrawLine(0.0f, y, APP_VIRTUAL_WIDTH, y);

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

void CRayCaster::RenderSprites(const CSimpleTileMap& map, const CViewer& viewer, const std::vector<CSprite>& sprites)
{
	std::map<float, const CSprite*> depthSortedSprites;
	for (auto& sprite : sprites) {
		//Frustum culling:
		CPoint toSprite{ sprite.position - viewer.m_position };
		float toSpriteDistance = Math::l2norm(toSprite);
		toSprite /= toSpriteDistance;

		//Return if the sprite isn't within the viewer's field of view.
		if (Math::dot(toSprite, Math::direction(viewer.m_angle)) <= cosf(Math::radians(viewer.m_fov) * 0.5f))
			continue;

		//Occlusion culling:
		CPoint poi = march(map, viewer.m_position, toSprite);
		float poiDistance = Math::l2norm(poi - viewer.m_position);

		//Return if the sprite is behind a wall.
		if (poiDistance < toSpriteDistance)
			continue;

		depthSortedSprites[toSpriteDistance] = &sprite;
	}

	for (auto it = depthSortedSprites.rbegin(); it != depthSortedSprites.rend(); ++it) {
		const CSprite& sprite = *it->second;

		//Reconstruct values.
		CPoint toSprite{ sprite.position - viewer.m_position };
		float toSpriteDistance = it->first;
		toSprite /= toSpriteDistance;

		//Angle between view direction and sprite.
		float spriteAngle = Math::radians(viewer.m_angle - Math::angle(toSprite));

		//Scale the sprite based on distance from the viewer.
		float scale = m_projectionDistance / abs(cosf(spriteAngle) * toSpriteDistance);

		//Figure out where the sprite lies horizontally relative to the centre of the screen based on the sprite angle and the projection plane triangle.
		float x = tanf(spriteAngle) * m_projectionDistance;

		//3D render.
		float xCentre = APP_VIRTUAL_WIDTH * 0.5f + x;
		float halfWidth = sprite.m_width * 0.5f * scale;
		float halfHeight = sprite.m_height * 0.5f * scale;
		App::DrawQuad(xCentre - halfWidth, m_rayOriginY - halfHeight, xCentre + halfWidth, m_rayOriginY + halfHeight, sprite.r, sprite.g, sprite.b);

#if DEBUG_DRAW
		glViewport(APP_VIRTUAL_WIDTH * 0.5f, 0.0f, APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT);
		App::DrawLine(viewer.m_position, sprite.position, sprite.r, sprite.g, sprite.b);
		App::DrawQuad(
			sprite.position.x - sprite.m_width * 0.5f, sprite.position.y - sprite.m_height * 0.5f,
			sprite.position.x + sprite.m_width * 0.5f, sprite.position.y + sprite.m_height * 0.5f,
			sprite.r, sprite.g, sprite.b
		);
		glViewport(0.0f, 0.0f, APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT);
#endif
	}
}

void CRayCaster::clearDepthBuffer()
{	//Over 9000 to ensure nothing will have a greater depth level.
	m_depthBuffer.assign(m_depthBuffer.size(), 9001.0f);
}

inline CPoint CRayCaster::march(const CSimpleTileMap& map, const CPoint& position, const CPoint& direction)
{
	const float unitRise = direction.y / direction.x;
	const float unitRun = direction.x / direction.y;

	//Continue searching until we find anything but a floor (air) or exceed the allowed amount of steps.
	EMapValue tileValue = EMapValue::AIR;
	uint32_t stepCount = 0;
	CPoint poi = position;
	while (tileValue == EMapValue::AIR) {
		float xOverlap = Math::overlap(poi.x, direction.x, map.GetTileWidth());
		const float xRate = xOverlap / direction.x;

		float yOverlap = Math::overlap(poi.y, direction.y, map.GetTileHeight());
		const float yRate = yOverlap / direction.y;

		//Increase the poi by a small percentage in order to ensure its in a new cell.
		static const float DISTANCE_MULTIPLIER = 1.05f;
		//Move x proportional to how we moved y or vice versa based on nearest edge.
		if (abs(yRate) < abs(xRate)) {
			const float distance = yOverlap * DISTANCE_MULTIPLIER;
			poi.y = poi.y + distance;
			poi.x = poi.x + distance * unitRun;
		}
		else {
			const float distance = xOverlap * DISTANCE_MULTIPLIER;
			poi.x = poi.x + distance;
			poi.y = poi.y + distance * unitRise;
		}

		//Look up cell.
		tileValue = map.GetTileMapValue(poi.x, poi.y);
		stepCount++;

		//Break out of the loop after too many steps (rare, but sometimes we get extreme numbers).
		if (stepCount > 32U) {
			tileValue = EMapValue::BORDER;
			break;
		}
	}

	return poi;
}