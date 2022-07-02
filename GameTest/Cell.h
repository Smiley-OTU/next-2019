#pragma once
struct Cell {
	int x;
	int y;

	bool operator==(const Cell& cell) const;
};

