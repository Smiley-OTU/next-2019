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
	void Debug(const CSimpleTileMap& map, const CViewer& viewer);

private:
	//At least I can pretend to use modern gl through naming conventions ;)
	std::vector<EMapValue> m_indexBuffer;
	std::vector<float> m_heightBuffer;
	//Stores range as an integer.
	const size_t m_count;
	const float m_thickness, m_step, m_rayOriginY;

	inline void march(const CSimpleTileMap& map, const CPoint& position, const CPoint& direction, size_t index);
};

