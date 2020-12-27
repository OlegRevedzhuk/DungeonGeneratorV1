#include <iostream>
#include <vector>
#include "constants.h"
#include "Dungeon.h"
#include "RngGen.h"
#include "StartingRoom.h"

enum class Sizes
{
	tiny,
	small,
	medium,
	large,
	huge,
	gargantuan,
};

int main()
{
	Dungeon dungeon{ 10, myConstants::rngRoomMinSize, myConstants::rngRoomMaxSize };

	std::cout << dungeon;

	return 0;
}