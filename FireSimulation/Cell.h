// SID: 1602819
// Fire Simulation Program Assignment.
//
// Contains the Cell class declarations.

#ifndef CELL_H
#define CELL_H

using namespace std;

// States a cell can be in
enum CellType
{
	// Possible cell spawn types
	Tree = 0,
	Empty = 1,

	// Other cell types
	BurningTree = 10,
	DeadTree = 11,
	HotEmbers = 12,
	Embers = 13

};

/// <summary>
/// 
///		Cell class creates the individual trees and fires, states can change
/// 
/// </summary>
class Cell
{
	private:
		CellType state;

		int BurnTime = 5;			// 5 turns
		int EmberTime = 2;			// 2 turns

		int caughtFire = false;
		int FireSpreadChance = 5;	// 5%

		int forestDensity = 100;	// 100%

	public:
		Cell();

		enum CellType GetState();
		void SetState(int index);

		int GetBurnTime();
		void SetBurnTime(int timeLeft);

		int GetEmberTime();
		void SetEmberTime(int timeLeft);

		bool GetCaughtFire();
		void SetCaughtFire(bool fire);
		
		int GetFireSpreadChance();
};

#endif
