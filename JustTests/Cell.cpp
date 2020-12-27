#include "Cell.h"

Cell::Cell(const SpaceType cellType) : m_cellType{ cellType } {}

char Cell::getCellDisplay() const
{
	switch (m_cellType)
	{
	case SpaceType::wall: return '0';
	case SpaceType::empty: return '1';
	case SpaceType::chest: return 'C';
	case SpaceType::door: return 'D';
	case SpaceType::player: return 'P';
	case SpaceType::trap: return 'T';
	case SpaceType::monster: return 'M';
	default: return '?';
	}
}

bool Cell::isEmpty() const
{
	if (m_cellType == SpaceType::empty)
		return true;
	else return false;
}

std::ostream& operator<<(std::ostream &out, const Cell &cell)
{
	out << cell.getCellDisplay();

	return out;
}