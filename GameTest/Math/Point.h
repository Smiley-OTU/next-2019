#pragma once
struct CPoint
{
	CPoint();
	CPoint(float a_x, float a_y);
	~CPoint();

	//Member variable access ie is [1].
	float operator[](unsigned int index);

	//Ideally prints are batched, but this is easier more convenient.
	void print();

	union {
		struct { float x, y; };
		struct { float values[2]; };
	};

private:
	static const unsigned int s_amount;
};