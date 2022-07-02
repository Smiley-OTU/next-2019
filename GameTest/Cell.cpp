#include "Cell.h"
#include "MutableCell.h"

MCell Cell::ToMCell() const
{
	MCell cell{ x, y };
	return cell;
}

bool Cell::operator==(const Cell& cell) const
{
	return x == cell.x && y == cell.y;
}
