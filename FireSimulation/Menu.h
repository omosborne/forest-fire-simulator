// SID: 1602819
// Fire Simulation Program Assignment.
//
// Contains the Menu class declarations.

#ifndef MENU_H
#define MENU_H
#include "Grid.h"

using namespace std;

// Options that can be chosen by the user on the main menu
enum MenuOption { Exit, Begin };

// Options that can be chosen by the user on the simulation menu
enum SimOption { Stop, Next };

/// <summary>
/// 
///		Menu class allows the user to interact with the simulation
/// 
/// </summary>
class Menu
{
private:
	MenuOption menuChoice;
	SimOption simChoice;
	

public:
	Menu();

	void DisplayMainMenu();
	void DisplaySimMenu(Grid* g);
	void DisplaySimOver();

	void MainMenu();
	void MainMenuError();

	void SimMenu(Grid* g);
	void SimOver();

	enum MenuOption GetMenuChoice();
	void SetMenuChoice(int input);
	
	enum SimOption GetSimChoice();
	void SetSimChoice(int input);
};

#endif