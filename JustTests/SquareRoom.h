#ifndef SQUARE_ROOM_H
#define SQUARE_ROOM_H

#include "Room.h"

class SquareRoom : public Room
{
public:
	SquareRoom(const int roomSize = 11);
	virtual ~SquareRoom() override;
};

#endif // !SQUARE_ROOM_H



