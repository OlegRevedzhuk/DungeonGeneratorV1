#include "DiamondRoom.h"

// operator?: is being used to make sure that DiamondRoom is always constructed with an odd size.
DiamondRoom::DiamondRoom(const int roomSize) : Room{ roomSize % 2 ? roomSize : roomSize + 1 }
{
	// we will create the room by quadrants with a 1 tile wide section in between them
	// m_roomSize/2 will be equal to m_roomSize/2 - 1 because of integral arithmetic (m_roomSize is odd)
	// this takes care of the top left quadrant
	for (int count{}; count < m_roomSize / 2; ++count)
	{
		for (int index{}; index < m_roomSize / 2; ++index)
		{
			if (index == m_roomSize / 2 - (count + 1))
				m_room[index].emplace_back(SpaceType::wall);
			else if (index < m_roomSize / 2 - (count + 1))
				m_room[index].emplace_back(SpaceType::none);
			else 
				m_room[index].emplace_back(SpaceType::empty);
		}
	}

	// here we add the 1 tile separating the top quadrants
	m_room[m_roomSize / 2].emplace_back(SpaceType::wall);
	for (int count{ 1 }; count < m_roomSize / 2; ++count)
		m_room[m_roomSize / 2].emplace_back(SpaceType::empty);

	// here we mirror the top left quadrant into the top right
	for (int count{ m_roomSize / 2 + 1 }; count < m_roomSize; ++count)
		for (int index{}; index < m_roomSize / 2; ++index)
			m_room[count].emplace_back(m_room[m_roomSize - (count + 1)][index]);

	// here we add the 1 tile separating the top and bottom halves
	for (int count{}; count < m_roomSize; ++count)
	{
		if (count == 0 || count == m_roomSize - 1)
			m_room[count].emplace_back(SpaceType::wall);
		else m_room[count].emplace_back(SpaceType::empty);
	}

	// here we mirror the top two quadrants into the bottom half, and are done
	for (int count{}; count < m_roomSize; ++count)
		for (int index{ m_roomSize / 2 + 1 }; index < m_roomSize; ++index)
			m_room[count].emplace_back(m_room[count][m_roomSize - (index + 1)]);

	if (m_hasAltar)
		m_room[m_roomSize / 2][m_roomSize / 2] = SpaceType::altar;

	setNumberOfEmpty();

	m_doors[static_cast<int>(Direction::north)] = { Direction::north, (&m_room[0][m_roomSize / 2]) };
	m_doors[static_cast<int>(Direction::east)] = { Direction::east,   (&m_room[m_roomSize / 2][m_roomSize - 1]) };
	m_doors[static_cast<int>(Direction::south)] = { Direction::south, (&m_room[m_roomSize - 1][m_roomSize / 2]) };
	m_doors[static_cast<int>(Direction::west)] = { Direction::west,   (&m_room[m_roomSize / 2][0]) };
}

DiamondRoom::~DiamondRoom()
{
}

bool DiamondRoom::hasAltar()
{
	return m_hasAltar;
}
