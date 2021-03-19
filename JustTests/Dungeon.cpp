#include "Dungeon.h"

Dungeon::Dungeon(const int numberOfRooms, const int minRoomSize, const int maxRoomSize) :
	m_dungeonSize{ numberOfRooms },
	m_map{ numberOfRooms }
{
	m_dungeonRooms.reserve(m_dungeonSize);
	m_allDoors.reserve(2 * m_dungeonSize);

	m_dungeonRooms.emplace_back(std::make_unique<StartingRoom>());

	for (int count{ 1 }; count < m_dungeonSize; ++count)
		m_dungeonRooms.emplace_back(std::make_unique<Room>(RngGen::randomNumberInRange(minRoomSize, maxRoomSize)));

	populateDungeon();

	connectDungeon();
}

void Dungeon::populateDungeon()
{
	for (int count{ 1 }; count < m_dungeonSize; ++count)
	{
		m_dungeonRooms[count]->populateTreasure();

		m_dungeonRooms[count]->populateMonster();
	}
}

void Dungeon::connectDungeon()
{
	addRoomsOnMap();

	addNonLinearConnections();

	addDoorsToRooms();
}

void Dungeon::addRoomsOnMap()
{
	std::vector<Room*> shuffledDungeon(m_dungeonRooms.size());
	std::transform(m_dungeonRooms.begin(), m_dungeonRooms.end(), shuffledDungeon.begin(), [](const auto& each) { 
																								return each.get(); });

	//shuffle rooms, but keep starting room at [0]. Do this to randomize order of room placement.
	std::shuffle(shuffledDungeon.begin() + 1, shuffledDungeon.end(), RngGen::getPRNG());

	//empty Map is generated with starting room (1) at (0, 0), additional rooms are added below.
	//set made to remember which rooms have maximum number of adjacent rooms.
	std::unordered_set<int> full;
	//another variable made to track number of rooms spawned from the starting room, in case we want to limit it
	int numberConnectedToStartingRoom{};

	//First we choose a room already on the map and a free direction from that room to place our new one.
	for (int index{ 1 }; index < m_dungeonSize; ++index)
	{
		int anchorIndex{};
		Direction dir;
		auto directionOrder{ RngGen::getDirectionOrder() };

		bool continueSearching{ true };
		while (continueSearching)
		{
			anchorIndex = RngGen::randomNumberInRange(0, index - 1);
			//check to see if anchorindex was previously found to be full
			if (full.contains(anchorIndex))
				continue;
			//make sure the anchor room has an empty adjacent tile and pick a direction
			for (int count{}; count < 4; ++count)
			{
				if (m_map.hasEmptyAdjacentNear(shuffledDungeon[anchorIndex]->getRoomID(), directionOrder[count]))
				{
					dir = directionOrder[count];
					continueSearching = false;
					break;
				}
			}
			if (continueSearching)
			{
				full.insert(anchorIndex);
			}
		}
		//check to see if we are attempting to add a room spawned off of starting room more times than the limit
		//potentially can add more limiting functionality here if needed
		if (anchorIndex == 0)
		{
			++numberConnectedToStartingRoom;
			if (numberConnectedToStartingRoom == myConstants::startingRoomMaxAdjacent)
				full.insert(anchorIndex);
		}

		//Now that a valid new location is found, we first update the map with the new roomID
		//and then create a connection between the rooms in all_doors
		int anchorRoomID{ shuffledDungeon[anchorIndex]->getRoomID() };
		int roomID{ shuffledDungeon[index]->getRoomID() };

		m_map.updateMapWithRoom(anchorRoomID, roomID, dir);

		m_allDoors.emplace_back(
			anchorRoomID,
			roomID,
			shuffledDungeon[anchorIndex]->getDoorPointer(dir),
			shuffledDungeon[index]->getDoorPointer(RngGen::getOppositeDirection(dir))
		);
	}
}

// here is where we customize how rooms that end up near each other but dont currently have a connection are treated
void Dungeon::addNonLinearConnections()
{
	for (int index{ 1 }; index < m_dungeonSize; ++index)
	{
		int roomID{ m_dungeonRooms[index]->getRoomID() };

		for (Direction dir{ Direction::north }; dir < Direction::totalDirections; ++dir)
		{
			if (!m_map.hasEmptyAdjacentNear(roomID, dir))
			{
				int adjacentRoomID{ m_map.getAdjacentRoomID(roomID, dir) };
				//not adding extra connections to starting room, so skip
				if (adjacentRoomID == myConstants::startingRoomID)
					continue;

				auto existingDoorConnection{ std::find_if(m_allDoors.begin(), m_allDoors.end(), [=](DoorStorage a) {
					return (a.firstRoomID == roomID && a.secondRoomID == adjacentRoomID)
						|| (a.firstRoomID == adjacentRoomID && a.secondRoomID == roomID); }) };

				//first non-starting room housed in dungeonRooms[1] has ID 2, so RoomID = index+1
				//below we want the index so index = roomid - 1
				if (existingDoorConnection == m_allDoors.end())
					m_allDoors.emplace_back(
						roomID,
						adjacentRoomID,
						m_dungeonRooms[index]->getDoorPointer(dir),
						m_dungeonRooms[adjacentRoomID - 1]->getDoorPointer(RngGen::getOppositeDirection(dir))
					);
			}
		}
	}
}

void Dungeon::addDoorsToRooms()
{
	for (auto& doorConnection : m_allDoors)
	{
		*doorConnection.firstDoor = SpaceType::door;
		*doorConnection.secondDoor = SpaceType::door;
	}
}

std::ostream& operator<<(std::ostream& out, const Dungeon& dungeon)
{
	for (int count{ 0 }; count < dungeon.m_dungeonSize; ++count)
		out << *dungeon.m_dungeonRooms[count].get();

	out << dungeon.m_map;

	return out;
}
