#include "Dungeon.h"

Dungeon::Dungeon(const int numberOfRooms, const int minRoomSize, const int maxRoomSize) :
	m_dungeonSize{ numberOfRooms - 1 }, //doesnt include starting room
	m_startingRoom{},
	m_map{ numberOfRooms }
{
	m_dungeonRooms.reserve(m_dungeonSize);

	for (int count{ }; count < m_dungeonSize; ++count)
		m_dungeonRooms.emplace_back(RngGen::randomNumberInRange(minRoomSize, maxRoomSize));

	populateDungeon();

	connectDungeon();
}

void Dungeon::populateDungeon()
{
	for (int count{}; count < m_dungeonSize; ++count)
	{
		m_dungeonRooms[count].populateTreasure();

		m_dungeonRooms[count].populateMonster();
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
	auto shuffledDungeon{ m_dungeonRooms };
	//tried and failed to move this shuffling to the RngGen class
	//RngGen::shuffleDungeon( shuffledDungeon );
	std::shuffle(shuffledDungeon.begin(), shuffledDungeon.end(), RngGen::getPRNG());
	m_allDoors.reserve(2 * m_dungeonSize);

	// always start with starting room (1) at (0, 0), we then iterate through all the rooms in dungeon
	for (int index{ }; index < m_dungeonSize; ++index)
	{
		int anchorIndex{};
		Direction dir;
		auto directionOrder{ RngGen::getDirectionOrder() };

		static int startingRoomNumOfAdjacentRooms{};

		//in this loop we choose a suitable location on the map to add the next room
		bool emptySpaceFound{};
		while (!emptySpaceFound)
		{
			// -1 represents starting room, choose anchor room index here
			anchorIndex = RngGen::randomNumberInRange(-1, index - 1);

			//make sure the anchor room has an empty adjacent tile
			for (int count{}; count < 4; ++count)
			{
				if (anchorIndex == -1)
				{
					//limit how many times starting room can be used as an anchor
					if (startingRoomNumOfAdjacentRooms >= myConstants::startingRoomMaxAdjacent)
						break;

					if (m_map.hasEmptyAdjacentNear(myConstants::startingRoomID, directionOrder[count]))
					{
						dir = directionOrder[count];
						emptySpaceFound = true;
						break;
					}
				}
				else if (m_map.hasEmptyAdjacentNear(shuffledDungeon[anchorIndex].getRoomID(), directionOrder[count]))
				{
					dir = directionOrder[count];
					emptySpaceFound = true;
					break;
				}
			}
		}
		int roomID{ shuffledDungeon[index].getRoomID() };

		// updating map here. We first set the roomID of the empty spot on map we chose to current id of index,
		// then we register the connection in our door storage.
		if (anchorIndex == -1)
		{
			m_map.updateMapWithRoom(myConstants::startingRoomID, roomID, dir);
			++startingRoomNumOfAdjacentRooms;

			m_allDoors.emplace_back(
				myConstants::startingRoomID,
				roomID,
				m_startingRoom.getDoorPointer(dir),
				shuffledDungeon[index].getDoorPointer(RngGen::getOppositeDirection(dir))
				);
		}
		else
		{
			int anchorRoomID{ shuffledDungeon[anchorIndex].getRoomID() };
			m_map.updateMapWithRoom(anchorRoomID, roomID, dir);

			m_allDoors.emplace_back(
				anchorRoomID,
				roomID,
				shuffledDungeon[anchorIndex].getDoorPointer(dir),
				shuffledDungeon[index].getDoorPointer(RngGen::getOppositeDirection(dir))
				);
		}
	}
}

// here is where we customize how rooms that end up near each other but dont currently have a connection are treated
void Dungeon::addNonLinearConnections()
{
	for (int index{ }; index < m_dungeonSize; ++index)
	{
		int roomID{ m_dungeonRooms[index].getRoomID() };

		for (Direction dir{Direction::north}; dir < Direction::totalDirections; ++dir)
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

				// -2 below because the first rng room housed in dungeonRooms[0] has ID 2, so m_dungeonRooms[index].getRoomID() = index+2
				if (existingDoorConnection == m_allDoors.end())
					m_allDoors.emplace_back(
						roomID,
						adjacentRoomID,
						m_dungeonRooms[index].getDoorPointer(dir),
						m_dungeonRooms[adjacentRoomID - 2].getDoorPointer(RngGen::getOppositeDirection(dir))
						);
			}
		}
	}
}

void Dungeon::addDoorsToRooms()
{
	for (auto &doorConnection : m_allDoors)
	{
		*doorConnection.firstDoor = SpaceType::door;
		*doorConnection.secondDoor = SpaceType::door;
	}
}

std::ostream & operator<<(std::ostream & out, Dungeon dungeon)
{
	out << dungeon.m_startingRoom;

	for (const auto &room : dungeon.m_dungeonRooms)
		out << room;

	out << dungeon.m_map;

	return out;
}
