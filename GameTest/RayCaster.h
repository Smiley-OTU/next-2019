#pragma once
#include <vector>
#include "Colour.h"
class CSimpleTileMap;
class CViewer;
class CRayCaster
{
public:
	CRayCaster(float = 1.0f);
	~CRayCaster();

	void Update();
	void Render(const CSimpleTileMap& map, const CViewer& viewer);

private:
	std::vector<CColour> m_colourBuffer;
	std::vector<float> m_heightBuffer;
	//Stores range as an integer.
	const size_t m_count;
	const float m_thickness, m_step, m_rayOriginY;

	inline void march(const CSimpleTileMap& map, const CPoint& position, const CPoint& direction);
};

