#pragma once
#include <vector>
#include "Colour.h"
//class CPoint;
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

	//Maps an index (0 to xMax) to (-1.0, 1.0).
	inline float indexToStep(size_t index);

	//Maps a step (-1.0, 1.0) to an index (0 to xMax).
	inline size_t stepToIndex(float step);

	inline void march(const CSimpleTileMap& map, const CPoint& position, const CPoint& direction);
};

