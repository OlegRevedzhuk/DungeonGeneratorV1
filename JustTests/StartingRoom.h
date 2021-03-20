#ifndef STARTINGROOM_H
#define STARTINGROOM_H

#include "Cell.h"
#include "SquareRoom.h"

class StartingRoom : 
	public SquareRoom
{
public:
	StartingRoom();
	virtual ~StartingRoom() override;
};

#endif // !STARTINGROOM_H



