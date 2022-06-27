#pragma once
#include "Cell.h"
#include "stdafx.h"
#define DISPLAY 1

inline int manhattan(const Cell& a, const Cell& b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

inline int euclidean(const Cell& a, const Cell& b)
{
	int dx = a.x - b.x;
	int dy = a.y - b.y;
	return (int)sqrt(dx * dx + dy * dy);
}

// f = g + h
// g = cost to move from 
// h = 
//inline int cost(const Cell& a, const Cell& b)
//{
//	return manhattan(a, b);
//}

struct Node
{
	Node()
	{
		init();
	}

	Node(const Cell& p)
	{
		init(p.x, p.y);
	}

	Node(const Cell& p, const Cell& pp)
	{
		init(p.x, p.y, pp.x, pp.y);
	}

	Node(const Cell& p, const Cell& pp, int f_, int g_, int h_)
	{
		init(p.x, p.y, pp.x, pp.y, f_, g_, h_);
	}

	Cell position() { return { x, y }; }
	Cell parent() { return { px, py }; }

	void init(int x_ = -1, int y_ = -1, int px_ = -1, int py_ = -1, int f_ = 0, int g_ = 0, int h_ = 0)
	{
		x = x_; y = y_; px = px_; py = py_; f = f_; g = g_; h = g_;
	}

	int x, y, px, py, f, g, h;
};

inline bool operator<(const Node& a, const Node& b) { return a.f < b.f; }