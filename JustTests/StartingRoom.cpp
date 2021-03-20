#include "StartingRoom.h"

StartingRoom::StartingRoom() : SquareRoom{ myConstants::startingRoomSize }
{
	m_room[m_roomSize / 4][m_roomSize / 2] = Cell(SpaceType::player);

	m_room[(m_roomSize * 3) / 4][m_roomSize / 4] = Cell(SpaceType::chest);
	m_room[(m_roomSize * 3) / 4][(m_roomSize * 2) / 4] = Cell(SpaceType::chest);
	m_room[(m_roomSize * 3) / 4][(m_roomSize * 3) / 4] = Cell(SpaceType::chest);
}

StartingRoom::~StartingRoom()
{
}
