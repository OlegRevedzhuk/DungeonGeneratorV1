#ifndef RNGGEN_H
#define RNGGEN_H

#include <array>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include "constants.h"

class Room;

enum class Direction
{
	north,
	east,
	south,
	west,
	totalDirections,
};

class RngGen
{
private:
	static std::mt19937 s_rng;
	static std::normal_distribution<> s_norm;
	static int s_currentNormMean;
	static std::array<Direction, 4> s_currentDirectionOrder;

private:
	static unsigned int randomNumber();

public:
	static std::mt19937 getPRNG();

	static bool percentChanceOfOccurring(const unsigned int percent);

	static int randomNumberInRange(const int min, const int max);

	static int distanceToNextCell(const int roomSize, const int numberOfObjects);

	static int numberOfChests();

	//static void shuffleDungeon(std::vector<Room>& dungeon);

	static std::array<Direction, 4>* getDirectionOrder();

	static Direction getOppositeDirection(Direction dir);
};

#endif // !RNGGEN_H
