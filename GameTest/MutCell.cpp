#include "MutCell.h"
#include "Cell.h"

Cell MCell::ToCell() const
{
	Cell cell{ x, y };
	return cell;
}

bool MCell::operator==(const MCell& cell) const
{
	return x == cell.x && y == cell.y;
}
