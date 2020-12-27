#ifndef CELL_H
#define CELL_H

#include <iostream>

enum class SpaceType
{
	wall,
	empty,
	chest,
	door,
	player,
	trap,
	monster,
	unknown,
};

class Cell
{
private:
	SpaceType m_cellType;

public:
	Cell(const SpaceType cellType = SpaceType::unknown);

private:
	char getCellDisplay() const;

	friend std::ostream& operator<<(std::ostream &out, const Cell &cell);

public:
	bool isEmpty() const;
};

#endif // !CELL_H