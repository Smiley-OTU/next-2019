#pragma once
//Cannot be made constant since we will be frequently reassigning them.
//Faster to do so rather than reallocate memory and re-initialize.
//Points & Lines will probably have to become non-constant too.
struct CColour {
	CColour() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	CColour(float a_r, float a_g, float a_b) : r(a_r), g(a_g), b(a_b), a(1.0f) {}

	void Randomize();
	static CColour Random();

	union {
		struct {
			float r, g, b, a;
		};
		struct {
			float values[4];
		};
	};
};