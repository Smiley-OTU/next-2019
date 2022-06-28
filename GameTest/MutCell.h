#pragma once
// Mutable Cell. Created so that implementing A* isn't a nightmare due to all the copy-assignments...
struct Cell;
struct MCell {
	int x;
	int y;

	Cell ToCell() const;
	bool operator==(const MCell& cell) const;
};