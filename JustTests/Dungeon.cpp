#include "Dungeon.h"

Dungeon::Dungeon(int numberOfRooms, int minRoomSize, int maxRoomSize) :
	m_map{ numberOfRooms },
	m_startingRoom{},
	m_dungeonSize{ numberOfRooms - 1 } //doesnt include starting room
{
	m_dungeonRooms.reserve(m_dungeonSize);

	for (int count{ }; count < m_dungeonSize; ++count)
		m_dungeonRooms.push_back(Room{ RngGen::randomNumberInRange(minRoomSize, maxRoomSize) });
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

	// always start with starting room (1) at (0, 0)
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
					if (startingRoomNumOfAdjacentRooms > myConstants::startingRoomMaxAdjacent)
						break;

					if (m_map.hasEmptyAdjacentNear(myConstants::startingRoomID, (*directionOrder)[count]))
					{
						dir = (*directionOrder)[count];
						emptySpaceFound = true;
						break;
					}
				}
				else if (m_map.hasEmptyAdjacentNear(shuffledDungeon[anchorIndex].getRoomID(), (*directionOrder)[count]))
				{
					dir = (*directionOrder)[count];
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

			m_allDoors.push_back({
				myConstants::startingRoomID,
				roomID,
				m_startingRoom.getDoorPointer(dir),
				shuffledDungeon[index].getDoorPointer(RngGen::getOppositeDirection(dir))
				});
		}
		else
		{
			m_map.updateMapWithRoom(shuffledDungeon[anchorIndex].getRoomID(), roomID, dir);

			m_allDoors.push_back({
				shuffledDungeon[anchorIndex].getRoomID(),
				roomID,
				shuffledDungeon[anchorIndex].getDoorPointer(dir),
				shuffledDungeon[index].getDoorPointer(RngGen::getOppositeDirection(dir))
				});
		}
	}
}

// here is where we customize how rooms that end up near each other but dont currently have a connection are treated
void Dungeon::addNonLinearConnections()
{
	for (int index{ }; index < m_dungeonSize; ++index)
	{
		int roomID{ m_dungeonRooms[index].getRoomID() };

		for (int dir{}; dir < static_cast<int>(Direction::totalDirections); ++dir)
		{
			if (!m_map.hasEmptyAdjacentNear(roomID, static_cast<Direction>(dir)))
			{
				int adjacentRoomID{ m_map.getAdjacentRoomID(roomID, static_cast<Direction>(dir)) };
				//not adding extra connections to starting room, so skip
				if (adjacentRoomID == myConstants::startingRoomID)
					continue;

				auto existingDoorConnection{ std::find_if(m_allDoors.begin(), m_allDoors.end(), [=](DoorStorage a) {
					return (a.firstRoomID == roomID && a.secondRoomID == adjacentRoomID)
						|| (a.firstRoomID == adjacentRoomID && a.secondRoomID == roomID); }) };

				// -2 below because the first rng room housed in dungeonRooms[0] has ID 2, so m_dungeonRooms[index].getRoomID() = index+2
				if (existingDoorConnection == m_allDoors.end())
					m_allDoors.push_back({
						m_dungeonRooms[index].getRoomID(),
						adjacentRoomID,
						m_dungeonRooms[index].getDoorPointer(static_cast<Direction>(dir)),
						m_dungeonRooms[adjacentRoomID - 2].getDoorPointer(RngGen::getOppositeDirection(static_cast<Direction>(dir)))
						});
			}
		}
	}
}

void Dungeon::addDoorsToRooms()
{
	for (int index{ }; index < m_allDoors.size(); ++index)
	{
		*m_allDoors[index].firstDoor = SpaceType::door;
		*m_allDoors[index].secondDoor = SpaceType::door;
	}
}

void Dungeon::printDungeon() const
{
	m_startingRoom.print();

	for (const auto &room : m_dungeonRooms)
		room.print();

	m_map.printDungeonMap();
}
