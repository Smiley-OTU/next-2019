#pragma once
#include "SimpleTileMap.h"
class CViewer;
class CRayCaster
{
public:
	CRayCaster(float = 1.0f);
	~CRayCaster();

	void Update(const CSimpleTileMap& map, const CViewer& viewer);
	void Render(const CSimpleTileMap& map, const CViewer& viewer);
	void RenderSprite(const CSimpleTileMap& map, const CViewer& viewer, const CPoint& spritePosition);
	//Tempted to to name this to clear, give it bitfield argument so I could type clear(GL_DEPTH_BUFFER_BIT) xD
	void clearDepthBuffer();

private:
	//At least I can pretend to use modern gl through naming conventions ;)
	std::vector<EMapValue> m_indexBuffer;
	std::vector<float> m_depthBuffer;

	//Stores intersection points for debugging.
	std::vector<CPoint> m_poiBuffer;

	//Number of rays.
	const uint32_t m_count;

	//Values that depend on each other at initialization time.
	const float m_thickness, m_step, m_rayOriginY;

	//Length of adjacent side of right triangle formed by the screen and the field of view.
	float m_projectionDistance;

	inline CPoint march(const CSimpleTileMap& map, const CPoint& position, const CPoint& direction);
};

