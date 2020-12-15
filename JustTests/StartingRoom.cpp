#include "StartingRoom.h"

StartingRoom::StartingRoom() : Room{ myConstants::startingRoomSize }
{
	m_room[3][6] = Cell(SpaceType::player);

	m_room[8][3] = Cell(SpaceType::chest);
	m_room[8][6] = Cell(SpaceType::chest);
	m_room[8][9] = Cell(SpaceType::chest);
}