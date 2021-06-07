// SID: 1602819
// Fire Simulation Program Assignment.
//
// Contains the Menu class method implementations.

#include "Menu.h"
#include "Grid.h"
#include <iostream>
#include <windows.h>

HANDLE  ConsoleMenuColour;

/// <summary>
/// 
///		Menu constructor
/// 
/// </summary>
Menu::Menu()
{
	// Used for colouring console text
	ConsoleMenuColour = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(ConsoleMenuColour);
}

/// <summary>
/// 
///		Show main menu, take user input, decide actions
/// 
/// </summary>
void Menu::DisplayMainMenu()
{
	
	int input;
	bool validChoice = false;

	// Collect the user input
	MainMenu();
	cin >> input;

	// Only allow input of 0 or 1, loop until user enters correctly
	while (!validChoice)
	{
		if (!(input == Exit || input == Begin))
		{
			// Display an error to the user if they make a mistake
			MainMenuError();
			cin >> input;
		}
		else
		{
			// Save valid inputs
			validChoice = true;
			SetMenuChoice(input);
		}
	}
}

/// <summary>
/// 
///		Show simulation menu, take user input, decide actions
/// 
/// </summary>
void Menu::DisplaySimMenu(Grid* g)
{
	char input = ' ';

	// Collect the user input, any key can work
	SimMenu(g);
	cin.get(input);

	// If entered key is 0, return back to main menu
	if (input == '0') SetSimChoice(Stop);
	
}

/// <summary>
/// 
///		Show simulation over screen, let the user know the fire is out
/// 
/// </summary>
void Menu::DisplaySimOver()
{
	char input = ' ';

	// This holds the simulation on screen, any key will dismiss this
	SimOver();
	cin.get(input);

}

/// <summary>
/// 
///		Construct the main menu
/// 
/// </summary>
void Menu::MainMenu()
{
	system("CLS");
	cout << endl << endl << endl;
	cout << "[1] Begin Simulation" << endl;
	cout << endl;
	cout << "[0] Exit" << endl;
	cout << endl << endl << endl;
	cout << "Option: ";
}

/// <summary>
/// 
///		Construct the main menu error
/// 
/// </summary>
void Menu::MainMenuError()
{
	system("CLS");
	cout << endl << endl << endl;
	cout << "[1] Begin Simulation" << endl;
	cout << endl;
	cout << "[0] Exit" << endl;
	cout << endl << endl;
	cout << "Invalid option, try again." << endl;
	cout << "Option: ";
}

/// <summary>
/// 
///		Construct the simulation menu
/// 
/// </summary>
void Menu::SimMenu(Grid* g)
{
	cout << "| Trees Remaining: [";

	// More than half trees remaining = green, less than half = gold, less than quarter = red
	if (g->GetTreesRemaining() <= ((g->GetWidth() * g->GetHeight()) / 4)) SetConsoleTextAttribute(ConsoleMenuColour, 4); // Red
	else if (g->GetTreesRemaining() <= ((g->GetWidth() * g->GetHeight()) / 2)) SetConsoleTextAttribute(ConsoleMenuColour, 6); // Gold
	else if (g->GetTreesRemaining() > ((g->GetWidth() * g->GetHeight()) / 2)) SetConsoleTextAttribute(ConsoleMenuColour, 10); // Green

	// Display the total trees left, then reset colour back
	cout << g->GetTreesRemaining();
	SetConsoleTextAttribute(ConsoleMenuColour, 7); // Reset colour

	// If total trees is 3 digits, remove 2 spaces, 2 digits remove 1 space (aligns console border)
	if (g->GetTreesRemaining() >= 100) cout << "]                     |" << endl;
	else if (g->GetTreesRemaining() >= 10) cout << "]                      |" << endl;
	else if (g->GetTreesRemaining() >= 0) cout << "]                      |" << endl;

	cout << "|                                            |" << endl;
	cout << "|                                            |" << endl;
	cout << "+--------------------------------------------+" << endl;
	cout << "  [ENTER] Next Stage" << endl;
	cout << "  [0]     Stop                     Option: ";
}

/// <summary>
/// 
///		Construct the simulation over screen
/// 
/// </summary>
void Menu::SimOver()
{
	cout << "|                                            |" << endl;
	cout << "|               ";
	SetConsoleTextAttribute(ConsoleMenuColour, 4); // Red
	cout << "SIMULATION OVER";
	SetConsoleTextAttribute(ConsoleMenuColour, 7); // Reset colour
	cout << "              |" << endl;
	cout << "|                                            |" << endl;
	cout << "+--------------------------------------------+" << endl;
	cout << endl;
	cout << "  Press [ENTER] to exit            Option: ";
}

// Get and Set methods below

enum MenuOption Menu::GetMenuChoice()
{
	return menuChoice;
}

void Menu::SetMenuChoice(int index)
{
	menuChoice = MenuOption(index);
}

enum SimOption Menu::GetSimChoice()
{
	return simChoice;
}

void Menu::SetSimChoice(int index)
{
	simChoice = SimOption(index);
}
