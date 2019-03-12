#pragma once
//Cannot be made constant since we will be frequently reassigning them.
//Faster to do so rather than reallocate memory and re-initialize.
//Points & Lines will probably have to become non-constant too.
struct CColour {
	CColour();
	CColour(float a_r, float a_g, float a_b);

	static CColour Random();
	void Randomize();
	float operator[](unsigned char index);

	union {
		struct {
			float r, g, b, a;
		};
		struct {
			float values[4];
		};
	};

	static const unsigned char s_components;
};