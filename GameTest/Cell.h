#pragma once
// Cell. This should've been the only x-y grid index representation for the benefit of immutability but then A* happened...
struct MCell;
struct Cell {
	const int x;
	const int y;

	MCell ToMCell() const;
	bool operator==(const Cell& cell) const;
};

