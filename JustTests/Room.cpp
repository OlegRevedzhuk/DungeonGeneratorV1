#include "Room.h"

int Room::s_nextRoomID{1};

Room::Room(const int roomSize) : m_roomID{ s_nextRoomID }, m_doors{
	{	{ Direction::north },
		{ Direction::east },
		{ Direction::south },
		{ Direction::west } } }
{
	++s_nextRoomID;
	// Expanding the capacity of our room to desired amount to reduce number of expansions later
	m_room.reserve(roomSize);

	for (int count{}; count < roomSize; ++count)
	{
		m_room.push_back({ Cell(SpaceType::wall) });
		m_room[count].reserve(roomSize);
	}

	// Allocating each member of the grid one at a time, top wall is already created above
	// The room is square in shape, has empty blocks everywhere except at the edges which are walls
	for (int count{}; count < roomSize; ++count)
	{
		for (int index{ 1 }; index < roomSize; ++index)
		{
			if ((count == 0) || (count == roomSize - 1) || (index == roomSize - 1))
				m_room[count].push_back({ Cell(SpaceType::wall) });
			else
				m_room[count].push_back({ Cell(SpaceType::empty) });
		}
	}

	//keeping track of how many empty cells there are
	for (const auto &row : m_room)
		m_numberOfEmptyCells += std::count_if(row.begin(), row.end(), [](Cell a) {
		if (a.getCellDisplay() == Cell{ SpaceType::empty }.getCellDisplay())
			return true;
		else return false; });

	//I dont know of a better way to initialize the pointer since its not generated until after this constructor.
	m_doors[static_cast<int>(Direction::north)].m_doorPointer = (&m_room[0][RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)]);
	m_doors[static_cast<int>(Direction::east)].m_doorPointer = (&m_room[RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)][roomSize - 1]);
	m_doors[static_cast<int>(Direction::south)].m_doorPointer = (&m_room[roomSize - 1][RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)]);
	m_doors[static_cast<int>(Direction::west)].m_doorPointer = (&m_room[RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)][0]);
}

void Room::populateMonster(int chance) // default = 85
{
	bool hasMonster{ RngGen::percentChanceOfOccurring(chance) };

	if (hasMonster)
	{
		int monsterCount{ RngGen::randomNumberInRange(myConstants::minMonsterCount, myConstants::maxMonsterCount) };
		int totalMonsters{ monsterCount };

		assert(monsterCount <= m_numberOfEmptyCells && "Not enough space to fit monsters.");

		int cellsToNextMonster{ RngGen::distanceToNextCell(m_room.size() - 2, totalMonsters) };

		while (monsterCount > 0)
		{
			for (int count{ 1 }; count < m_room.size() - 1; ++count)
			{
				for (int index{ 1 }; index < m_room[count].size() - 1; ++index)
				{
					//when current Cell is to be assigned
					if (!--cellsToNextMonster)
					{
						//check if Cell is empty
						if (m_room[count][index].getCellDisplay() != Cell(SpaceType::empty).getCellDisplay())
						{
							++cellsToNextMonster;
							continue;
						}

						//50% assign Cell, 50% check opposite cell and assign, or move to next Cell if not empty
						//50% to spread out monsters evenly at starting cell and ending cell
						if (RngGen::percentChanceOfOccurring(50))
						{
							m_room[count][index] = Cell(SpaceType::monster);
							--m_numberOfEmptyCells;
						}
						else
						{
							if (m_room[index][count].getCellDisplay() != Cell(SpaceType::empty).getCellDisplay())
							{
								++cellsToNextMonster;
								continue;
							}
							m_room[index][count] = Cell(SpaceType::monster);
							--m_numberOfEmptyCells;
						}
						--monsterCount;

						if (monsterCount)
							cellsToNextMonster = RngGen::distanceToNextCell(m_room.size() - 2, totalMonsters);
					}
					if (!monsterCount)
						break;
				}
				if (!monsterCount)
					break;
			}
		}
	}
}

void Room::populateTreasure(int chance) // default = 25
{
	bool hasTreasure{ RngGen::percentChanceOfOccurring(chance) };

	if (hasTreasure)
	{
		int chests{ RngGen::numberOfChests() };

		assert(chests <= m_numberOfEmptyCells && "no room for treasure");

		auto size{ m_room.size() };

		for (int count{ 1 }; count <= chests; ++count)
		{
			m_room[size / 2][size / (chests + 1)*(count)] = Cell{ SpaceType::chest };
			--m_numberOfEmptyCells;
			populateTrap(size / 2, size / (chests + 1)*(count));
		}
	}
}

void Room::populateTrap(int x, int y, int chance) // default = 20
{
	bool hasTrap{ RngGen::percentChanceOfOccurring(chance) };

	if (hasTrap)
	{
		if (m_room[x + 1][y].getCellDisplay() == Cell(SpaceType::empty).getCellDisplay())
		{
			m_room[x + 1][y] = Cell(SpaceType::trap);
			--m_numberOfEmptyCells;
		}

		if (m_room[x][y + 1].getCellDisplay() == Cell(SpaceType::empty).getCellDisplay())
		{
			m_room[x][y + 1] = Cell(SpaceType::trap);
			--m_numberOfEmptyCells;
		}

		if (m_room[x - 1][y].getCellDisplay() == Cell(SpaceType::empty).getCellDisplay())
		{
			m_room[x - 1][y] = Cell(SpaceType::trap);
			--m_numberOfEmptyCells;
		}

		if (m_room[x][y - 1].getCellDisplay() == Cell(SpaceType::empty).getCellDisplay())
		{
			m_room[x][y - 1] = Cell(SpaceType::trap);
			--m_numberOfEmptyCells;
		}
	}
}

int Room::getRoomID() const
{
	return m_roomID;
}

Cell * Room::getDoorPointer(const Direction dir) const
{
	return (*std::find_if(m_doors.begin(), m_doors.end(), [=](Door a) {
		return a.m_direction == dir; })).m_doorPointer;
}

void Room::print() const
{
	std::cout << "Room #" << m_roomID << '\n';

	for (int index{}; index < m_room.size(); ++index)
	{
		for (size_t count{}; count < m_room[index].size(); ++count)
			std::cout << m_room[index][count].getCellDisplay() << ' ';

		std::cout << '\n';
	}
	std::cout << '\n';
}