#include "SquareRoom.h"

SquareRoom::SquareRoom(const int roomSize) : Room{ roomSize }
{
	// Allocating each member of the grid one at a time
	// The room is square in shape, has empty blocks everywhere except at the edges which are walls
	for (int count{}; count < roomSize; ++count)
	{
		for (int index{}; index < roomSize; ++index)
		{
			if ((count == 0) || (count == roomSize - 1) || (index == roomSize - 1) || (index == 0))
				m_room[count].emplace_back(SpaceType::wall);
			else
				m_room[count].emplace_back(SpaceType::empty);
		}
	}

	setNumberOfEmpty();

	//random number range chooses a random door along the room's wall.
	//not sure if it would be better to move the random number range choosing to RngGen and use a switch.
	//my goal with the min and max ranges is to make sure that the door is not in a courner
	m_doors[static_cast<int>(Direction::north)] = { Direction::north, (&m_room[0][RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)]) };
	m_doors[static_cast<int>(Direction::east)] = { Direction::east,   (&m_room[RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)][roomSize - 1]) };
	m_doors[static_cast<int>(Direction::south)] = { Direction::south, (&m_room[roomSize - 1][RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)]) };
	m_doors[static_cast<int>(Direction::west)] = { Direction::west,   (&m_room[RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)][0]) };
}

SquareRoom::~SquareRoom()
{
}
