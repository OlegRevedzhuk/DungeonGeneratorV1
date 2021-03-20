#ifndef ROOM_H
#define ROOM_H

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>
#include "Cell.h"
#include "constants.h"
#include "RngGen.h"

class Room
{
private:
	using GridType_t = std::vector<std::vector<Cell>>;

	struct Door
	{
		Direction m_direction;
		Cell* m_doorPointer;
	};

private:
	static int s_nextRoomID;
	int m_roomID;

protected:
	int m_roomSize;
	GridType_t m_room;
	std::array<Door, 4> m_doors;
	int m_numberOfEmptyCells;

public:
	Room(const int roomSize);
	virtual ~Room();

	void populateMonster(const int chance = 85);
	void populateTreasure(const int chance = 25);

	int getRoomID() const;
	Cell* getDoorPointer(const Direction dir) const;

protected:
	void setNumberOfEmpty();

private:
	void populateTrap(const int x, const int y, const int chance = 20);

	virtual bool hasAltar();

	friend std::ostream& operator<< (std::ostream &out, const Room &room);
};

#endif // !ROOM_H