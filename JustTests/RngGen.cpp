#include "RngGen.h"

std::mt19937 RngGen::s_rng{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

std::normal_distribution<> RngGen::s_norm{};

int RngGen::s_currentNormMean{};

std::array<Direction, 4> RngGen::s_currentDirectionOrder{ {Direction::north, Direction::east, Direction::south, Direction::west} };

Direction& operator++(Direction &dir)
{
	if (dir == Direction::totalDirections)
		return dir;

	dir = static_cast<Direction>(static_cast<int>(dir) + 1);
	return dir;
}

std::mt19937 RngGen::getPRNG()
{
	return s_rng;
}

bool RngGen::percentChanceOfOccurring(const unsigned int percent)
{
	if ((s_rng() % 100) < percent)
		return true;
	else
		return false;
}

int RngGen::randomNumberInRange(const int min, const int max)
{
	return (s_rng() % (max - min + 1)) + min;
}

int RngGen::distanceToNextCell(const int roomSize, const int numberOfObjects)
{
	if (s_currentNormMean != static_cast<int>(roomSize * roomSize / (numberOfObjects + 0.5)))
	{
		s_currentNormMean = static_cast<int>(roomSize * roomSize / (numberOfObjects + 0.5));
		s_norm = std::normal_distribution<>{ roomSize * roomSize / (numberOfObjects + 0.5) , roomSize * roomSize / (numberOfObjects + 0.5) };
	}
	int distance = static_cast<int>(std::lround(s_norm(s_rng)));
	
	if (distance > 0)
		return distance;
	else
		return 1;
}

int RngGen::numberOfChests()
{
	if (percentChanceOfOccurring(10))
		return 2;
	else
		return 1;
}

/*
void RngGen::shuffleDungeon(std::vector<Room>& dungeon)
{
	std::shuffle(dungeon.begin(), dungeon.end(), s_rng);
}
*/

const std::array<Direction, 4>& RngGen::getDirectionOrder()
{
	std::shuffle(s_currentDirectionOrder.begin(), s_currentDirectionOrder.end(), s_rng);
	return s_currentDirectionOrder;
}

Direction RngGen::getOppositeDirection(Direction dir)
{
	switch (dir)
	{
	case Direction::north: return Direction::south;
	case Direction::east: return Direction::west;
	case Direction::south: return Direction::north;
	case Direction::west: return Direction::east;
	default: std::cout << "something went wrong";
	}
}
