#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace myConstants
{
	// minimum 13, includes walls, if updated populateStartingRoom() needs updates
	constexpr int startingRoomSize{ 13 };
	constexpr int startingRoomID{ 1 };

	// includes walls
	constexpr int rngRoomMinSize{ 7 };
	constexpr int rngRoomMaxSize{ 15 };
	constexpr int rngDungeonDefaultSize{ 10 };

	constexpr int minMonsterCount{ 3 };
	constexpr int maxMonsterCount{ 5 };

	constexpr int altarChance{ 10 };

	//1 min
	constexpr int startingRoomMaxAdjacent{ 2 };
}
#endif