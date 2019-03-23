#pragma once
struct CColour {
	CColour();
	CColour(float a_r, float a_g, float a_b);

	static CColour Random();
	void Randomize();
	float operator[](unsigned char index);

	union {
		struct {
			float r, g, b;//, a;
		};
		struct {
			float values[3];//[4];
		};
	};

	static const unsigned char s_components;
};