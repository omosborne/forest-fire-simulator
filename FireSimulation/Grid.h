// SID: 1602819
// Fire Simulation Program Assignment.
//
// Contains the Grid class declarations.

#ifndef GRID_H
#define GRID_H
#include "Cell.h"

using namespace std;

/// <summary>
/// 
///		Grid class creates the board where cells are displayed and the visuals for what the user sees
/// 
/// </summary>
class Grid
{
private:
	int width = 21;				// 21 cells
	int height = 21;			// 21 cells

	Cell cells[21][21];			// 2D array 21x21 cells

	bool FireStarted = false;

	int treesRemaining = 0;

	bool simState = false;
	bool simOver = false;

public:
	Grid();

	void GenSeed();
	void CreateGrid();
	void DrawGrid();
	void DrawGridBorder(char position);
	void DrawCell(CellType state);
	void UpdateCells();
	void CheckSimEnd();

	void StartFire();
	bool HasFireStarted();
	void AttemptFireSpread(int i, int j);
	void FireSpread(Cell* neighbour);
	void UpdateFireSpread();
	
	Cell* GetCell(int x, int y);

	int GetWidth();
	int GetHeight();

	int GetTreesRemaining();
	void SetTreesRemaining(int trees);
	
	void SetSimState(bool simulationStatus);

	bool GetSimState();
	void SetSimOver(bool simulationOver);

	bool GetSimOver();	
};

#endif
