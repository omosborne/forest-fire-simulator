// SID: 1602819
// Fire Simulation Program Assignment.
//
// Contains the Grid class method implementations.

#include "Grid.h"
#include "Cell.h"
#include <iostream>
#include <windows.h>

HANDLE  ConsoleGridColour;

/// <summary>
/// 
///		Grid constructor creates a new seed and cells to fill a grid
/// 
/// </summary>
Grid::Grid()
{
	// Used for colouring console text
	ConsoleGridColour = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(ConsoleGridColour);

	// Create a random number seed for the current grid and create the cells
	GenSeed();
	CreateGrid();
}

void Grid::GenSeed()
{
	srand((unsigned)time(NULL));
}

/// <summary>
/// 
///		Create the cell objects for the grid, add to 2D array using a nested for loop
/// 
/// </summary>
void Grid::CreateGrid()
{
	// Used to determine end of simulation
	SetSimState(true);

	// Loop through all cells
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Cell *c = new Cell();
			
			// Create Fire Break (Empty cells around edges)
			if (i == 0) c->SetState(Empty);
			else if (j == 0) c->SetState(Empty);
			else if (j == width - 1) c->SetState(Empty);
			else if (i == height - 1) c->SetState(Empty);

			// Use 2D Array to store cell objects
			cells[i][j] = *c;

			// Accumulate a total counter for trees to be displayed on the HUD
			if (c->GetState() == Tree) SetTreesRemaining(GetTreesRemaining() + 1);
		}
	}
}

/// <summary>
/// 
///		Draw the states of the cells to the screen
/// 
/// </summary>
void Grid::DrawGrid()
{
	system("CLS");

	// Top white border
	DrawGridBorder('T');

	// Loop through all cells
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// Left white border
			if (j == 0) DrawGridBorder('L');
			
			Cell* c = &cells[i][j];

			// Determine what state the cell is in, then draw the correct ascii
			switch (c->GetState())
			{
				case Empty:
					DrawCell(Empty);
					break;

				case Tree:
					DrawCell(Tree);
					break;

				case BurningTree:
					DrawCell(BurningTree);
					break;

				case DeadTree:
					DrawCell(DeadTree);
					break;

				case HotEmbers:
					DrawCell(HotEmbers);
					break;

				case Embers:
					DrawCell(Embers);
					break;
			}
			// Right white border
			if (j == width - 1) DrawGridBorder('R');
		}
		SetConsoleTextAttribute(ConsoleGridColour, 7); // Reset colour
		cout << endl;
	}
	// Bottom white border
	DrawGridBorder('B');
	
}

/// <summary>
/// 
///		Draw a border around the whole grid to separate it from other UI
/// 
/// </summary>
void Grid::DrawGridBorder(char position)
{
	switch (position)
	{
		case 'T': // Top
			SetConsoleTextAttribute(ConsoleGridColour, 15); // White
			cout << "+--------------------------------------------+" << endl;
			break;

		case 'L': // Left
			SetConsoleTextAttribute(ConsoleGridColour, 15); // White
			cout << "| ";
			break;

		case 'R': // Right
			SetConsoleTextAttribute(ConsoleGridColour, 15); // White
			cout << " |";
			break;

		case 'B': // Bottom
			SetConsoleTextAttribute(ConsoleGridColour, 15); // White
			cout << "+--------------------------------------------+" << endl;
			break;
	}
}

/// <summary>
/// 
///		ASCII for each cell state, draws to screen when requested
/// 
/// </summary>
void Grid::DrawCell(CellType state)
{
	// Determine what ascii to draw for each state
	switch (state)
	{
		case Empty:
			SetConsoleTextAttribute(ConsoleGridColour, 7); // Reset colour
			cout << "  ";
			break;

		case Tree:
			SetConsoleTextAttribute(ConsoleGridColour, 10); // Green
			cout << "/\\";
			break;

		case BurningTree:
			SetConsoleTextAttribute(ConsoleGridColour, 4); // Red
			cout << "##";
			break;

		case DeadTree:
			SetConsoleTextAttribute(ConsoleGridColour, 8); // Grey
			cout << "..";
			break;

		case HotEmbers:
			SetConsoleTextAttribute(ConsoleGridColour, 14); // Yellow
			cout << "##";
			break;

		case Embers:
			SetConsoleTextAttribute(ConsoleGridColour, 6); // Gold
			cout << "##";
			break;
	}
}

/// <summary>
/// 
///		Brains behind simulation, checks all calculations for cell states and updates them for next grid draw
/// 
/// </summary>
void Grid::UpdateCells()
{
	// Used to determine end of simulation
	SetSimState(false);

	// Calculate each cell for changing states
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Cell* c = GetCell(i, j);

			// If a burning tree has been around for too long, turn it into hot embers
			if (c->GetState() == BurningTree && c->GetBurnTime() == 0)
			{
				c->SetState(HotEmbers);
			}
			// Otherwise decrease the total turns it will keep burning for
			else if (c->GetState() == BurningTree)
			{
				c->SetBurnTime(c->GetBurnTime() - 1);
				if (c->GetCaughtFire()) c->SetCaughtFire(false);
				
				// Try to burn surrounding trees
				AttemptFireSpread(i, j);
			}

			// Turn a hot ember into a regular ember as it loses heat
			if (c->GetState() == HotEmbers && c->GetEmberTime() == 1)
			{
				c->SetState(Embers);
			}
			// If it's new hot ember, let it last another turn
			else if (c->GetState() == HotEmbers)
			{
				c->SetEmberTime(c->GetEmberTime() - 1);
			}

			// When ember burns out, turn into a dead tree
			if (c->GetState() == Embers && c->GetEmberTime() == 0)
			{
				c->SetState(DeadTree);
			}
			// If it's a new regular ember, let it last another turn
			else if (c->GetState() == Embers)
			{
				c->SetEmberTime(c->GetEmberTime() - 1);
			}
		}
	}
	// If any trees have been calculated to catch fire, update their state now
	UpdateFireSpread();

	// Check if any trees or embers are still around, if not, simulation is over
	CheckSimEnd();
}

/// <summary>
/// 
///		Used to identify the end of the simulation
/// 
/// </summary>
void Grid::CheckSimEnd()
{
	// Check if any trees or embers are still around, if so, keep simulation state active
	// (update cells method will auto set this to false until no fires/embers can set it to true again, sim over)
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Cell* c = GetCell(i, j);

			if (c->GetState() == BurningTree || c->GetState() == HotEmbers || c->GetState() == Embers)
			{
				SetSimState(true);
			}
		}
	}
}

/// <summary>
/// 
///		Begin a random fire
/// 
/// </summary>
void Grid::StartFire()
{
	// Determine a random start location
	int startX = rand() % width;
	int startY = rand() % height;

	Cell* c = GetCell(startY, startX);

	// If the chosen cell is a tree, burn it
	if (c->GetState() == Tree)
	{
		c->SetState(BurningTree);
		SetTreesRemaining(GetTreesRemaining() - 1);
	}

	// If any of the cells are a burning tree, let the simulation know it's started
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Cell* c = GetCell(i, j);

			if (c->GetState() == BurningTree)
			{
				FireStarted = true;
			}
		}
	}
}

bool Grid::HasFireStarted()
{
	return FireStarted;
}

/// <summary>
/// 
///		Detects neighbours and tries to set them alight
/// 
/// </summary>
void Grid::AttemptFireSpread(int i, int j)
{
	// Cell edge detection to prevent out of index error
	if (i == 0 && j == 0)
	{
		// Top left neighbour cell identification

		// -- -- --
		// -- [] n3
		// -- n5 n4

		// Collect cell reference for each relative position to the focused cell.

		Cell* n3 = GetCell(i, j + 1);
		Cell* n4 = GetCell(i + 1, j + 1);
		Cell* n5 = GetCell(i + 1, j);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n3);
		FireSpread(n4);
		FireSpread(n5);
	}
	else if (i == 0 && j == width - 1)
	{
		// Top right neighbour cell identification

		// -- -- --
		// n7 [] --
		// n6 n5 --

		// Collect cell reference for each relative position to the focused cell.

		Cell* n5 = GetCell(i + 1, j);
		Cell* n6 = GetCell(i + 1, j - 1);
		Cell* n7 = GetCell(i, j - 1);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n5);
		FireSpread(n6);
		FireSpread(n7);
	}
	else if (i == height - 1 && j == width - 1)
	{
		// Bottom right neighbour cell identification

		// n0 n1 --
		// n7 [] --
		// -- -- --

		// Collect cell reference for each relative position to the focused cell.

		Cell* n0 = GetCell(i - 1, j - 1);
		Cell* n1 = GetCell(i - 1, j);
		Cell* n7 = GetCell(i, j - 1);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n0);
		FireSpread(n1);
		FireSpread(n7);
	}
	else if (i == height - 1 && j == 0)
	{
		// Bottom left neighbour cell identification

		// -- n1 n2
		// -- [] n3
		// -- -- --

		// Collect cell reference for each relative position to the focused cell.

		Cell* n1 = GetCell(i - 1, j);
		Cell* n2 = GetCell(i - 1, j + 1);
		Cell* n3 = GetCell(i, j + 1);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n1);
		FireSpread(n2);
		FireSpread(n3);
	}
	else if (i == 0)
	{
		// Top edge neighbour cell identification

		// -- -- --
		// n7 [] n3
		// n6 n5 n4

		// Collect cell reference for each relative position to the focused cell.

		Cell* n3 = GetCell(i, j + 1);
		Cell* n4 = GetCell(i + 1, j + 1);
		Cell* n5 = GetCell(i + 1, j);
		Cell* n6 = GetCell(i + 1, j - 1);
		Cell* n7 = GetCell(i, j - 1);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n3);
		FireSpread(n4);
		FireSpread(n5);
		FireSpread(n6);
		FireSpread(n7);
	}
	else if (j == width - 1)
	{
		// Right edge neighbour cell identification

		// n0 n1 --
		// n7 [] --
		// n6 n5 --

		// Collect cell reference for each relative position to the focused cell.

		Cell* n0 = GetCell(i - 1, j - 1);
		Cell* n1 = GetCell(i - 1, j);
		Cell* n5 = GetCell(i + 1, j);
		Cell* n6 = GetCell(i + 1, j - 1);
		Cell* n7 = GetCell(i, j - 1);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n0);
		FireSpread(n1);
		FireSpread(n5);
		FireSpread(n6);
		FireSpread(n7);
	}
	else if (i == height - 1)
	{
		// Bottom edge neighbour cell identification

		// n0 n1 n2
		// n7 [] n3
		// -- -- --

		// Collect cell reference for each relative position to the focused cell.

		Cell* n0 = GetCell(i - 1, j - 1);
		Cell* n1 = GetCell(i - 1, j);
		Cell* n2 = GetCell(i - 1, j + 1);
		Cell* n3 = GetCell(i, j + 1);
		Cell* n7 = GetCell(i, j - 1);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n0);
		FireSpread(n1);
		FireSpread(n2);
		FireSpread(n3);
		FireSpread(n7);
	}
	else if (j == 0)
	{
		// Left edge neighbour cell identification

		// -- n1 n2
		// -- [] n3
		// -- n5 n4

		// Collect cell reference for each relative position to the focused cell.

		Cell* n1 = GetCell(i - 1, j);
		Cell* n2 = GetCell(i - 1, j + 1);
		Cell* n3 = GetCell(i, j + 1);
		Cell* n4 = GetCell(i + 1, j + 1);
		Cell* n5 = GetCell(i + 1, j);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n1);
		FireSpread(n2);
		FireSpread(n3);
		FireSpread(n4);
		FireSpread(n5);
	}
	else
	{
		// Neighbour cell identification for non-edge cells
	
		// n0 n1 n2
		// n7 [] n3
		// n6 n5 n4

		// Collect cell reference for each relative position to the focused cell.

		Cell* n0 = GetCell(i - 1, j - 1);
		Cell* n1 = GetCell(i - 1, j);
		Cell* n2 = GetCell(i - 1, j + 1);
		Cell* n3 = GetCell(i, j + 1);
		Cell* n4 = GetCell(i + 1, j + 1);
		Cell* n5 = GetCell(i + 1, j);
		Cell* n6 = GetCell(i + 1, j - 1);
		Cell* n7 = GetCell(i, j - 1);

		// Attempt to spread the fire to the below neighbours.

		FireSpread(n0);
		FireSpread(n1);
		FireSpread(n2);
		FireSpread(n3);
		FireSpread(n4);
		FireSpread(n5);
		FireSpread(n6);
		FireSpread(n7);
	}
}

/// <summary>
/// 
///		Set trees on fire logically first
///		(prevents fires created now from being immediately updated when loop continues)
/// 
/// </summary>
void Grid::FireSpread(Cell* neighbour)
{
	// If neighbour is a tree
	if (neighbour->GetState() == Tree)
	{
		// Get their change of catching fire
		int spreadChance = neighbour->GetFireSpreadChance(); // 5%
		int spread = rand() % (100 - 1);

		// If they catch fire
		if (spread <= spreadChance)
		{
			// Set object fire attribute for updating the fire spread later
			neighbour->SetCaughtFire(true);
		}
	}
	
}

/// <summary>
/// 
///		If a tree is logically on fire, update its state to actually be on fire
/// 
/// </summary>
void Grid::UpdateFireSpread()
{
	// Loop through all cells in 2D array
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// Create local reference cell
			Cell* c = GetCell(i, j);

			// If cell was previously set on fire
			if (c->GetCaughtFire())
			{
				// Update cell state to be on fire for next grid draw.
				c->SetState(BurningTree);
				SetTreesRemaining(GetTreesRemaining() - 1);
			}
		}
	}
}


// Get and Set methods below

Cell* Grid::GetCell(int y, int x)
{
	Cell* c = &cells[y][x];
	return c;
}

int Grid::GetWidth()
{
	return width;
}

int Grid::GetHeight()
{
	return height;
}

int Grid::GetTreesRemaining()
{
	return treesRemaining;
}

void Grid::SetTreesRemaining(int trees)
{
	treesRemaining = trees;
}

void Grid::SetSimState(bool simulationStatus)
{
	simState = simulationStatus;
}

bool Grid::GetSimState()
{
	return simState;
}

void Grid::SetSimOver(bool simulationOver)
{
	simOver = simulationOver;
}

bool Grid::GetSimOver()
{
	return simOver;
}