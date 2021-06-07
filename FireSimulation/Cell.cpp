// SID: 1602819
// Fire Simulation Program Assignment.
//
// Contains the Cell class method implementations.

#include "Cell.h"
#include <cstdlib>
#include <time.h>

// class methods here, constructor etc

/// <summary>
/// 
///		Cell constructor, determine what state the cell spawns as
/// 
/// </summary>
Cell::Cell()
{
	// If random number between 0 and 99 (100 chances) is less than forest density, spawn a tree
	int TreeSpawnChance = rand() % 100 - 1;
	if (TreeSpawnChance <= forestDensity) SetState(Tree);
	else SetState(Empty);
}

// Get and Set methods below

enum CellType Cell::GetState()
{
	return state;
}

void Cell::SetState(int index)
{
	state = CellType(index);
}

int Cell::GetBurnTime()
{
	return BurnTime;
}

void Cell::SetBurnTime(int timeLeft)
{
	BurnTime = timeLeft;
}

int Cell::GetEmberTime()
{
	return EmberTime;
}

void Cell::SetEmberTime(int timeLeft)
{
	EmberTime = timeLeft;
}

bool Cell::GetCaughtFire()
{
	return caughtFire;
}

void Cell::SetCaughtFire(bool fire)
{
	caughtFire = fire;
}

int Cell::GetFireSpreadChance()
{
	return FireSpreadChance;
}