#include "DungeonMap.h"

//the number of rooms y that radius x can hold is y = (2x+1)^2, so we solve for y and add padding for varience to determine m_mapRadius
//the padding starts at 5 at base case, as the array gets larger there can be slightly more varience so we add extra using a log3.
//this works well until really high number of rooms, still have to think about how to better allocate right amount of map if ever want to do hundreds of rooms.
DungeonMap::DungeonMap(int numberOfRooms) : 
	m_mapRadius{ 3 + static_cast<int>(std::log10(numberOfRooms)/std::log10(3)) + (static_cast<int>(std::sqrt(numberOfRooms)) - 1) / 2 }, 
	m_mapLayout{ {0,0,1} }
{
	m_mapLayout.reserve((2 * m_mapRadius + 1)*(2 * m_mapRadius + 1));

	int arrayIndex{ 1 };

	//here we add mapRadius amount of layers to the map starting at mapLayout[0] representing the starting room at (0, 0) and working outward
	//next entry will be mapLayout[1] at (1, 1) and then we go counter clockwise.
	for (int count{ 1 }; count <= m_mapRadius; ++count)
	{
		for (int index{ 0 }; index < (2 * count + 1); ++index)
		{
			m_mapLayout.push_back({ count - index, count });
			++arrayIndex;
		}

		for (int index{ 1 }; index < (2 * count + 1); ++index)
		{
			m_mapLayout.push_back({ (-count), count - index });
			++arrayIndex;
		}

		for (int index{ 1 }; index < (2 * count + 1); ++index)
		{
			m_mapLayout.push_back({ (-count) + index, (-count) });
			++arrayIndex;
		}

		for (int index{ 1 }; index < (2 * count); ++index)
		{
			m_mapLayout.push_back({ count, (-count) + index });
			++arrayIndex;
		}
	}
}

int DungeonMap::idOfRoomAtCoords(const int x, const int y) const
{
	return Coords{ *std::find_if(m_mapLayout.begin(), m_mapLayout.end(), [=](Coords coords) {
					return (coords.x == x) && (coords.y == y); }) }.currentAssignedRoom;
}

int DungeonMap::getAdjacentRoomID(const int roomID, const Direction dir) const
{
	const Coords& coord{ *std::find_if(m_mapLayout.begin(), m_mapLayout.end(), [=](Coords a) {
		return a.currentAssignedRoom == roomID; }) };

	switch (dir)
	{
	case Direction::north:	return idOfRoomAtCoords(coord.x, coord.y + 1);
	case Direction::east:	return idOfRoomAtCoords(coord.x + 1, coord.y);
	case Direction::south:	return idOfRoomAtCoords(coord.x, coord.y - 1);
	case Direction::west:	return idOfRoomAtCoords(coord.x - 1, coord.y);
	default:return false;
	}
}

bool DungeonMap::hasEmptyAdjacentNear(const int roomID, const Direction dir) const
{
	if (getAdjacentRoomID(roomID, dir) > 0)
		return false;
	else return true;
}

void DungeonMap::updateMapWithRoom(const int anchorRoomID, const int newRoomID, const Direction directionFromAnchor)
{
	const auto anchorRoom{ std::find_if(m_mapLayout.begin(), m_mapLayout.end(), [=](Coords a) {
		return a.currentAssignedRoom == anchorRoomID; }) };

	switch (directionFromAnchor)
	{
	case Direction::north:
		std::find_if(m_mapLayout.begin(), m_mapLayout.end(), [=](Coords a) {
			return a.x == anchorRoom->x && a.y == anchorRoom->y + 1; })->currentAssignedRoom = newRoomID;
		break;
	case Direction::east:
		std::find_if(m_mapLayout.begin(), m_mapLayout.end(), [=](Coords a) {
			return a.x == anchorRoom->x + 1 && a.y == anchorRoom->y; })->currentAssignedRoom = newRoomID;
		break;
	case Direction::south:
		std::find_if(m_mapLayout.begin(), m_mapLayout.end(), [=](Coords a) {
			return a.x == anchorRoom->x && a.y == anchorRoom->y - 1; })->currentAssignedRoom = newRoomID;
		break;
	case Direction::west:
		std::find_if(m_mapLayout.begin(), m_mapLayout.end(), [=](Coords a) {
			return a.x == anchorRoom->x - 1 && a.y == anchorRoom->y; })->currentAssignedRoom = newRoomID;
		break;
	default: std::cout << "something went wrong\n";
	}
}

void DungeonMap::printDungeonMap() const
{
	std::cout << "Dungeon room layout:\n";

	for (int index{}; index < m_mapRadius * 2 + 1; ++index)
	{
		for (int count{}; count < m_mapRadius * 2 + 1; ++count)
		{
			int state{ idOfRoomAtCoords(-m_mapRadius + count, m_mapRadius - index) };

			if (state > 0 && state < 10)
				std::cout << ' ' << state << ' ';
			else if (state >= 10)
				std::cout << ' ' << state;
			else
				std::cout << "   ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}