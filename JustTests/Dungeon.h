#ifndef DUNGEON_H
#define DUNGEON_H

#include <algorithm>
#include <memory>
#include <unordered_set>
#include <vector>
#include "constants.h"
#include "DiamondRoom.h"
#include "DungeonMap.h"
#include "Room.h"
#include "RngGen.h"
#include "StartingRoom.h"
#include "SquareRoom.h"

struct DungeonParameters
{
	int numberOfRooms{ myConstants::rngDungeonDefaultSize };
	int minRoomSize{ myConstants::rngRoomMinSize };
	int maxRoomSize{ myConstants::rngRoomMaxSize };
	//additional parameters in future
};

class Dungeon
{
private:
	struct DoorStorage
	{
		int firstRoomID;
		int secondRoomID;
		Cell* firstDoor;
		Cell* secondDoor;
	};

private:
	int m_dungeonSize;
	std::vector<std::unique_ptr<Room>> m_dungeonRooms;
	DungeonMap m_map;
	std::vector<DoorStorage> m_allDoors;

public:
	Dungeon(const DungeonParameters params);

private:
	void populateDungeon();

	void connectDungeon();

	void addRoomsOnMap();

	void addNonLinearConnections();

	void addDoorsToRooms();

	friend std::ostream& operator<< (std::ostream& out, const Dungeon& dungeon);
};

#endif // !DUNGEON_H
