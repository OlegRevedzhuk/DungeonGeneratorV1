#include "Room.h"

int Room::s_nextRoomID{ myConstants::startingRoomID };

Room::Room(const int roomSize) : m_roomID{ s_nextRoomID++ }, m_roomSize{ roomSize }, m_numberOfEmptyCells{}, m_doors{}
{
	// Expanding the capacity of our room to desired amount to reduce number of expansions later
	m_room.reserve(roomSize);

	for (int count{}; count < roomSize; ++count)
	{
		m_room.emplace_back(std::vector<Cell>(0));
		m_room[count].reserve(roomSize);
	}
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

	// To select which cell a monster goes in we will iterate through the open cells in our room
	// cellsToNextMonster will decide how many cells we skip before trying to assign monsters
	int cellsToNextMonster{ RngGen::distanceToNextCell(m_numberOfEmptyCells, totalMonsters) };

	// infinite loop to make sure that if we go through the entire grid without assigning all monsters, we restart at the top
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

					//50% assign Cell, 50% check cell as if we were searching in reverse 
					//assign selected Cell, or move to next Cell if not empty
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
						cellsToNextMonster = RngGen::distanceToNextCell(m_numberOfEmptyCells, totalMonsters);
					else return;
				}
			}
		}
	}
}

//If we end up needing special treasure populating based on room type, can move to derived using virtual f(x)
void Room::populateTreasure(const int chance) // default = 25
{
	bool hasTreasure{ RngGen::percentChanceOfOccurring(chance) };

	if (hasAltar())
		return;

	if (!hasTreasure)
		return;

	int chests{ RngGen::numberOfChests() };

	assert(chests <= m_numberOfEmptyCells && "no room for treasure");

	for (int count{ 1 }; count <= chests; ++count)
	{
		//placing chests in middle row of the room, equally spaced appart
		m_room[m_roomSize / 2][m_roomSize / (chests + 1) * (count)] = Cell{ SpaceType::chest };
		--m_numberOfEmptyCells;
		populateTrap(m_roomSize / 2, m_roomSize / (chests + 1) * (count));
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

Cell* Room::getDoorPointer(const Direction dir) const
{
	return (*std::find_if(m_doors.begin(), m_doors.end(), [=](Door a) {
		return a.m_direction == dir; })).m_doorPointer;
}

void Room::setNumberOfEmpty()
{
	//count how many empty cells there are
	int empty{};
	for (const auto& row : m_room)
		empty += std::count_if(row.begin(), row.end(), [](Cell a) {
		if (a.isEmpty())
			return true;
		else return false; });

	m_numberOfEmptyCells = empty;
}

bool Room::hasAltar() const
{
	return false;
}

std::ostream& operator<<(std::ostream& out, const Room& room)
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