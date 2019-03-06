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

	//Can't have these if we want our members to be constant.
	//void operator+=(const CPoint&);
	//void operator-=(const CPoint&);
	//void operator*=(float);
	//void operator/=(float);

	//Member variable access ie y is [1], max of [1].
	float operator[](unsigned int index) const;

	union {
		struct { const float x, y; };
		struct { const float values[2]; };
	};

	static const unsigned int s_components;
};