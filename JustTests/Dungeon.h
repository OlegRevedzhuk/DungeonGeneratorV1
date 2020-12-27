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

		DoorStorage(const int firstID, const int secondID, Cell* const firstPointer, Cell* const secondPointer)
			: firstRoomID{ firstID }, secondRoomID{ secondID }, firstDoor{ firstPointer }, secondDoor{ secondPointer } {}
	};

private:
	int m_dungeonSize;
	StartingRoom m_startingRoom;
	std::vector<Room> m_dungeonRooms;
	DungeonMap m_map;
	std::vector<DoorStorage> m_allDoors;

public:
	Dungeon(const int numberOfRooms, const int minRoomSize, const int maxRoomSize);

private:
	void populateDungeon();

	void connectDungeon();

	void addRoomsOnMap();

	void addNonLinearConnections();

	void addDoorsToRooms();

	friend std::ostream& operator<< (std::ostream &out, Dungeon dungeon);
};

#endif // !DUNGEON_H



