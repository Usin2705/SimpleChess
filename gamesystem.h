// Manage the system of the game, including print out menu, new game, load game, save game, quit game .. blah .. blah
// Anymethod that change the system of the game, without need access to the chestboard or board.h should be put here.

#pragma once

#include <iostream>
#include <vector>
#include "constant.h"

class GameSystem {
	friend wostream &operator<<(wostream &out, const GameSystem &gamesystem);	// To print out the menu
public:
	GameSystem(int _scene = MENUSCENE);										
	int GetScene() const;
	void SetScene(int);
private:
	int scene;

};

/*
Print out the menu, as well as handle mouse click in GetMouseInput.
Depend on the return from mouse click, set the game state correct with user input.
*/
void PlayMenu(GameSystem &gamesytem);								

/*
Print out the How to play game.

When you change stage from different font size, the smaller font size won't display correctly.
A "quick" fix is to windowed the console and then go full screen again.

Then if the user click on MENU then return to menu
*/
void PlayHowToPlay(GameSystem &gamesytem);

/*
Handle error from other input
NOT SURE IF THIS IS NEEDED.
*/
void ErrorExit(LPSTR, HANDLE, DWORD);

/*
Get Mouse input from the user.
Each mouse input read will return 1 pair int, int. The first int is the column, then second int is the row
Notice that XCORD = COLUMN and YCORD = ROW

For wide character, 1 wide character = 2 normal character, so when convert to COLUMN from XCORD, we need to divide by 2

Notice that to handle mouse input, we need to disable 'Quick Edit Mode', which require the ENABLE_EXTENDED_FLAGS

For now, the game don't handle any input aside from mouse, so we can ignore any other input. And by ignore I mean
to put break; in those case. Since I also handle stranger input with error message.
*/
pair<int, int> GetMouseInput();
