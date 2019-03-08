#pragma once
#include <vector>
#include "Colour.h"
class CRayCaster
{
public:
	CRayCaster(float, float, float = 1.0f);
	~CRayCaster();

	void Update();
	void Render();

	//Vertical position of rays.
	float rayOriginY;

private:
	std::vector<CColour> m_colourBuffer;
	std::vector<float> m_heightBuffer;
	const float m_xMin, m_xMax, m_range, m_step, m_thickness;
	//Stores range as an integer. Not completely redundant cause it improves safety.
	const size_t m_count;

	//Maps an index (0 to xMax) to (-1.0, 1.0).
	inline float mapToStep(size_t index);

	//Maps a step (-1.0, 1.0) to an index (0 to xMax).
	inline size_t mapToIndex(float step);
};

