//***TLDR this class shouldn't exist. Its in place to see where it could go, but its more code duplication and wasted storage than anything...***
#pragma once
#include "Point.h"
#include "Line.h"
//Most likely won't be used because there's no need to store excessive memory.
//ie Figure out the colour of the tile the ray hits and set the render colour to that.
//(Don't need to store if we play our cards right). 
struct CColouredLine
{
	CColouredLine(float a_p1x, float a_p1y, float a_p2x, float a_p2y, float a_r = 1.0f, float a_g = 1.0f, float a_b = 1.0f, float a_a = 1.0f);
	CColouredLine(const CPoint& a_p1, const CPoint& a_p2, float a_r = 1.0f, float a_g = 1.0f, float a_b = 1.0f, float a_a = 1.0f);
	CColouredLine(const CLine& a_line, float a_r = 1.0f, float a_g = 1.0f, float a_b = 1.0f, float a_a = 1.0f);
	//CColouredLine(float values[8]);//Not safe and not useful at the moment.
	CColouredLine();
	~CColouredLine();

	//Coloured lines are frequently created so it makes sense to have a mechanism to verify their status. 
	bool initialized();

	//Pretty much it, although this is a sort of bad idea as mentioned above.
	//Perhaps it could be used sparingly for debugging, or projectiles, but the temptation to mis-use this is too strong as of now!
	//void render();

	//Member variable access ie y is [1], max of [7].
	float operator[](unsigned int index) const;

	//Immutable for now for defensive programming.
	//Alpha is padding.
	union {
		struct {
			const float p1x, p1y, p2x, p2y;
			const float r, g, b, a;
		};
		struct {
			const CPoint p1, p2;
			const float r, g, b, a;
		};
		struct {
			const CLine line;
			const float r, g, b, a;
		};
		struct {
			const float values[8];
		};
	};
	//Starting to get ugly (r, g, b, a duplicated between layouts).
	//Inheritance isn't the answer, and templates aren't applicable due to different member amounts & names.
	//Could do a silly thing where we define x as values[0], which is templatable! Try that out in a separate project before setting this in stone!

	const static unsigned int s_components;
};