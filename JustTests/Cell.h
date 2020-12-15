#ifndef CELL_H
#define CELL_H

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

	char getCellDisplay() const;
};

#endif // !CELL_H