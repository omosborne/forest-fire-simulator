// SID: 1602819
// Fire Simulation Program Assignment.
//
// Contains the Main method. Program is run from this file.

#include "Menu.h"
#include "Grid.h"
#include <iostream>
using namespace std;

/// <summary>
/// 
///		Main program loop is run from here.
/// 
/// </summary>
int main(void)
{
	system("TITLE Fire Simulation SID1602819");

	// Create a new menu and grid, this resets
	// the simulation with recursion later on
	Menu* menu = new Menu();
	Grid* grid = new Grid();
	
	menu->DisplayMainMenu();

	// Decide which action the user wants to take
	switch (menu->GetMenuChoice())
	{
	case Exit:
		break;

	case Begin:

		// Keep looping the simulation until the user commands to stop
		while (menu->GetSimChoice() != Stop)
		{
			// First loop to ensure a fire starts
			while (grid->HasFireStarted() == false)
			{
				grid->DrawGrid();
				menu->DisplaySimMenu(grid);

				// Allow the user to cancel the simulation at this stage if they want to
				if (menu->GetSimChoice() != Stop) grid->StartFire();
				else break;
			}
			if (menu->GetSimChoice() == Stop) break;

			// Main loop for the simulation
			grid->DrawGrid();
			menu->DisplaySimMenu(grid);

			// As long as the user does not stop, the simulation will keep updating
			if (menu->GetSimChoice() != Stop) grid->UpdateCells();
			
			// Identify if all fires have burnt out and not embers remain, simulation is over
			if (grid->GetSimState() == false)
			{
				grid->SetSimOver(true);
				break;
			}
			
		}
		if (grid->GetSimOver() == true)
		{
			grid->DrawGrid();
			menu->DisplaySimOver();
		}

		// recursively loop the program so a new seed is generated for a new grid
		main();

	}

	return 0;
}