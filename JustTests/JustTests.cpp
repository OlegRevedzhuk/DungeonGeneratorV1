#include <iostream>
#include <vector>
#include "constants.h"
#include "Dungeon.h"
#include "RngGen.h"
#include "StartingRoom.h"

/*
enum class Sizes
{
	tiny,
	small,
	medium,
	large,
	huge,
	gargantuan,
};
*/

int main()
{
	int dungeonSize{};
	std::cout << "How many rooms would you like to include? ";
	std::cin >> dungeonSize;

	Dungeon dungeon{ dungeonSize, myConstants::rngRoomMinSize, myConstants::rngRoomMaxSize };

	std::cout << dungeon;

	return 0;
}