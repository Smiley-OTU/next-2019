#pragma once
#include "SimpleTileMap.h"
#include "Sprite.h"
class CViewer;
class CRayCaster
{
public:
	CRayCaster(float = 1.0f);
	~CRayCaster();

	void Update(const CSimpleTileMap& map, const CViewer& viewer);
	void RenderMap(const CSimpleTileMap& map, const CViewer& viewer);
	void RenderSprites(const CSimpleTileMap& map, const CViewer& viewer, const Sprites& sprites);
	void ClearDepthBuffer();

private:
	//At least I can pretend to use modern gl through naming conventions ;)
	std::vector<EMapValue> m_indexBuffer;
	std::vector<float> m_depthBuffer;

	//Number of rays.
	const uint32_t m_count;

	//Values that depend on each other at initialization time.
	const float m_thickness, m_rayOriginY;

	//Length of adjacent side of right triangle formed by the screen and the field of view.
	float m_projectionDistance;

	inline CPoint March(const CSimpleTileMap& map, const CPoint& position, const CPoint& direction);
};
