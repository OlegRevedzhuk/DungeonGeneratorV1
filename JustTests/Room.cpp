#include "Room.h"

int Room::s_nextRoomID{ 1 };

Room::Room(const int roomSize) : m_roomID{ s_nextRoomID++ }, m_roomSize{roomSize}
{
	// Expanding the capacity of our room to desired amount to reduce number of expansions later
	m_room.reserve(roomSize);

	for (int count{}; count < roomSize; ++count)
	{
		m_room.emplace_back(std::vector<Cell>{ SpaceType::wall });
		m_room[count].reserve(roomSize);
	}

	// Allocating each member of the grid one at a time, top wall is already created above
	// The room is square in shape, has empty blocks everywhere except at the edges which are walls
	for (int count{}; count < roomSize; ++count)
	{
		for (int index{ 1 }; index < roomSize; ++index)
		{
			if ((count == 0) || (count == roomSize - 1) || (index == roomSize - 1))
				m_room[count].emplace_back(SpaceType::wall);
			else
				m_room[count].emplace_back(SpaceType::empty);
		}
	}

	//keeping track of how many empty cells there are
	for (const auto &row : m_room)
		m_numberOfEmptyCells += std::count_if(row.begin(), row.end(), [](Cell a) {
		if (a.isEmpty())
			return true;
		else return false; });

	//random number range chooses a random door along the room's wall.
	//not sure if it would be better to move the random number range choosing to RngGen and use a switch.
	//my goal with the min and max ranges is to make sure that the door is not in a courner
	m_doors[static_cast<int>(Direction::north)] = { Direction::north, (&m_room[0][RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)]) };
	m_doors[static_cast<int>(Direction::east)] = { Direction::east,   (&m_room[RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)][roomSize - 1]) };
	m_doors[static_cast<int>(Direction::south)] = { Direction::south, (&m_room[roomSize - 1][RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)]) };
	m_doors[static_cast<int>(Direction::west)] = { Direction::west,   (&m_room[RngGen::randomNumberInRange(roomSize / 4, (roomSize * 3) / 4)][0]) };
}

Room::~Room()
{
}

void Room::populateMonster(const int chance) // default = 85
{
	bool hasMonster{ RngGen::percentChanceOfOccurring(chance) };

	if (!hasMonster)
		return;

	int monsterCount{ RngGen::randomNumberInRange(myConstants::minMonsterCount, myConstants::maxMonsterCount) };
	int totalMonsters{ monsterCount };

	assert(totalMonsters <= m_numberOfEmptyCells && "Not enough space to fit monsters.");

	int cellsToNextMonster{ RngGen::distanceToNextCell(m_roomSize - 2, totalMonsters) };

	while (true)
	{
		for (int count{ 1 }; count < m_roomSize - 1; ++count)
		{
			for (int index{ 1 }; index < m_roomSize - 1; ++index)
			{
				//when we have skipped enough empty cells
				if (!--cellsToNextMonster)
				{
					//if current cell is occupied try the next one
					if (!m_room[count][index].isEmpty())
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
						if (!m_room[m_roomSize - count][m_roomSize - index].isEmpty())
						{
							++cellsToNextMonster;
							continue;
						}
						m_room[m_roomSize - count][m_roomSize - index] = Cell(SpaceType::monster);
						--m_numberOfEmptyCells;
					}
					--monsterCount;

					if (monsterCount)
						cellsToNextMonster = RngGen::distanceToNextCell(m_roomSize - 2, totalMonsters);
					else return;
				}
			}
		}
	}
}

void Room::populateTreasure(const int chance) // default = 25
{
	bool hasTreasure{ RngGen::percentChanceOfOccurring(chance) };

	if (!hasTreasure)
		return;

	int chests{ RngGen::numberOfChests() };

	assert(chests <= m_numberOfEmptyCells && "no room for treasure");

	for (int count{ 1 }; count <= chests; ++count)
	{
		//placing chests in middle row of the room, equally spaced appart
		m_room[m_roomSize / 2][m_roomSize / (chests + 1)*(count)] = Cell{ SpaceType::chest };
		--m_numberOfEmptyCells;
		populateTrap(m_roomSize / 2, m_roomSize / (chests + 1)*(count));
	}
}

void Room::populateTrap(const int x, const int y, const int chance) // default = 20
{
	bool hasTrap{ RngGen::percentChanceOfOccurring(chance) };

	if (!hasTrap)
		return;

	if (m_room[x + 1][y].isEmpty())
	{
		m_room[x + 1][y] = Cell(SpaceType::trap);
		--m_numberOfEmptyCells;
	}

	if (m_room[x][y + 1].isEmpty())
	{
		m_room[x][y + 1] = Cell(SpaceType::trap);
		--m_numberOfEmptyCells;
	}

	if (m_room[x - 1][y].isEmpty())
	{
		m_room[x - 1][y] = Cell(SpaceType::trap);
		--m_numberOfEmptyCells;
	}

	if (m_room[x][y - 1].isEmpty())
	{
		m_room[x][y - 1] = Cell(SpaceType::trap);
		--m_numberOfEmptyCells;
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

std::ostream & operator<<(std::ostream & out, const Room & room)
{
	out << "Room #" << room.m_roomID << '\n';

	for (int index{}; index < room.m_roomSize; ++index)
	{
		for (int count{}; count < room.m_roomSize; ++count)
			out << room.m_room[index][count] << ' ';

		out << '\n';
	}
	out << '\n';

	return out;
}