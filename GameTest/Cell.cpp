#include "Cell.h"

bool Cell::operator==(const Cell& cell) const
{
	return x == cell.x && y == cell.y;
}
