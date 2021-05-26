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
	char useDefault{};
	std::cout << "Would you like to use default settings? (Y/N) ";
	std::cin >> useDefault;
	DungeonParameters params{};

	if (useDefault == 'N' || useDefault == 'n')
	{
		std::cout << "Desired number of rooms: ";
		std::cin >> params.numberOfRooms;
		std::cout << "Desired minimum room size (MIN " << myConstants::rngRoomMinSize << "): ";
		std::cin >> params.minRoomSize;
		if (params.minRoomSize < myConstants::rngRoomMinSize)
			params.minRoomSize = myConstants::rngRoomMinSize;
		std::cout << "Desired maximum room size (MAX " << myConstants::rngRoomMaxSize << "): ";
		std::cin >> params.maxRoomSize;
		if (params.maxRoomSize > myConstants::rngRoomMaxSize)
			params.maxRoomSize = myConstants::rngRoomMaxSize;
	}
	std::cout << std::endl;

	Dungeon dungeon{ params };

	std::cout << dungeon;

	std::string x{};
	std::cout << "Each letter in the room displays represents a different object type.\n"
		<< "M = monster, A = altar, D = door, T = trap, C = chest, and P = player.\n";
	std::cin >> x;

	return 0;
}