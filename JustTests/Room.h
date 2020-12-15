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

using GridType_t = std::vector<std::vector<Cell>>;

class Room
{
private:
	struct Door
	{
		Direction m_direction;
		Cell* m_doorPointer;
	};

protected:
	static int s_nextRoomID;

	int m_roomID;
	GridType_t m_room;
	std::array<Door, 4> m_doors;
	int m_numberOfEmptyCells{};

public:
	Room(const int roomSize = 11);

	void populateMonster(int chance = 85);

	void populateTreasure(int chance = 25);

private:
	void populateTrap(int x, int y, int chance = 20);

public:
	int getRoomID() const;

	Cell* getDoorPointer(const Direction dir) const;

	void print() const;
};

#endif // !ROOM_H