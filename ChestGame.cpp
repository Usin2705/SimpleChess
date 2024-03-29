// ChestGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gamesystem.h"
#include "windows.h"
#include "board.h"

// BUG LIST:
// SCREEN NOT PRINTED OUT ALL BLACK. The color depend on the color of the last cell clicked on

/*
	The main method of the game.
	This chest game work based on the SCENE of the game.
	Depend on the SCENE type, the game will print out different thing. Menu for MENUSCENE, CHESTBOARD for LOADSCENE or NEWSCENE.
	If its the NEWSCENE then board will load a new board game, if it a LOADSCENE then board will load from SAVE FILE
*/
int main()
{
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0); // FULL SCREEN
	
	GameSystem gamesystem(MENUSCENE);
	
	// Declare a empty board, to save memory, this board 2D vector is empty.
	Board board;
	while (gamesystem.GetScene() != ENDSCENE) {

		// Print out the menu scene, until the GAMESCENE change
		while (gamesystem.GetScene() == MENUSCENE) {
			PlayMenu(gamesystem);
		}

		// Print out the how to play scene, until the GAMESCENE change
		while (gamesystem.GetScene() == HOWTOPLAYSCENE) {
			PlayHowToPlay(gamesystem);
		}
		
		// If the game is from load, then load board from file, else create new board.
		if (gamesystem.GetScene() == LOADSCENE) {
			board.LoadBoard(gamesystem);
		}
		else {
			board.LoadNewGame();
		}			
			
		// Print out the board and play, until the GAMESCENE change
		while (gamesystem.GetScene() == NEWSCENE|| gamesystem.GetScene() == LOADSCENE) {
			PlayBoard(board, gamesystem);
		}
	}

	system("pause");
	return 0;
}

