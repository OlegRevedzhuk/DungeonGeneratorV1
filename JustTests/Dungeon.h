#ifndef DUNGEON_H
#define DUNGEON_H

#include <algorithm>
#include <vector>
#include "constants.h"
#include "DungeonMap.h"
#include "Room.h"
#include "RngGen.h"
#include "StartingRoom.h"

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
	StartingRoom m_startingRoom;
	std::vector<Room> m_dungeonRooms;
	DungeonMap m_map;
	std::vector<DoorStorage> m_allDoors;

public:
	Dungeon(int numberOfRooms, int minRoomSize, int maxRoomSize);

	void populateDungeon();

	void connectDungeon();

private:
	void addRoomsOnMap();

	void addNonLinearConnections();

	void addDoorsToRooms();

public:
	void printDungeon() const;
};

#endif // !DUNGEON_H



