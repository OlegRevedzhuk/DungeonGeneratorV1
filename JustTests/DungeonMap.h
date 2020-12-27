#ifndef DUNGEONMAP_H
#define DUNGEONMAP_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "constants.h"
#include "Room.h"

class DungeonMap
{
private:
	struct Coords
	{
		int x;
		int y;
		int currentAssignedRoom;
	};

private:
	std::vector<Coords> m_mapLayout; //first entry preresents spawn room
	int m_mapRadius;

public:
	DungeonMap(const int numberOfRooms);

private:
	int idOfRoomAtCoords(const int x, const int y) const;

	friend std::ostream& operator<< (std::ostream &out, const DungeonMap &map);

public:
	int getAdjacentRoomID(const int roomID, const Direction dir) const;

	bool hasEmptyAdjacentNear(const int roomID, const Direction dir) const;

	void updateMapWithRoom(const int anchorRoomID, const int newRoomID, const Direction directionFromAnchor);
};

#endif // !DUNGEONMAP_H