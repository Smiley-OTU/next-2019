#pragma once
struct CPoint
{
	CPoint(float a_x, float a_y);
	//CPoint(float values[2]);//Not safe and not useful at the moment.
	CPoint();
	~CPoint();

	//Points are frequently created so it makes sense to have a mechanism to verify their status. 
	bool initialized() const;

	CPoint operator+(const CPoint&) const;
	CPoint operator-(const CPoint&) const;
	CPoint operator*(float) const;
	CPoint operator/(float) const;

	//Decided to go with no chaining (returning CPoint&s) to prevent error and preserve legibility.
	void operator+=(const CPoint&);
	void operator-=(const CPoint&);
	void operator*=(float);
	void operator/=(float);

	//Member variable access ie y is [1], max of [1].
	float operator[](unsigned char index) const;

	union {
		struct { float x, y; };
		struct { float values[2]; };
	};

	static const unsigned char s_components;
};