#ifndef DIAMOND_ROOM_H
#define DIAMOND_ROOM_H

#include "Room.h"

class DiamondRoom : public Room
{
private:
	bool m_hasAltar{RngGen::percentChanceOfOccurring(myConstants::altarChance)};

public:
	DiamondRoom(const int roomSize = 11);
	virtual ~DiamondRoom() override;

private:
	virtual bool hasAltar() override;
};

#endif // !DIAMOND_ROOM_H
