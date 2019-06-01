
#include "stdafx.h"
#include "gamesystem.h"

/*
	Print out the game menu
	This method don't use wcout. however, to return from wcout (from GAMESCENE), we need to use wcout, otherwise error will happen. 

	First set the font size 
	Then set the font type. Notice that to print out chest piece as unicode, only some font will work. MS Gothic is one
	Then set the text color.
*/
wostream & operator<<(wostream & out, const GameSystem &gamesystem)
{
	HANDLE outcon = GetStdHandle(STD_OUTPUT_HANDLE);		//you don't have to call this function every time
	CONSOLE_FONT_INFOEX font;								//CONSOLE_FONT_INFOEX is defined in some windows header	
	GetCurrentConsoleFontEx(outcon, false, &font);			//PCONSOLE_FONT_INFOEX is the same as CONSOLE_FONT_INFOEX*
	font.cbSize = sizeof(font);								//Need to set cbSize
	font.dwFontSize.X = 0;
	font.dwFontSize.Y = FZ_OPTION;
	font.FontWeight = FW_EXTRABOLD;
	wcscpy_s(font.FaceName, L"MS Gothic");					// Choose your font
	SetCurrentConsoleFontEx(outcon, false, &font);
	SetConsoleTextAttribute(outcon, CL_YELLOW);

	wcout << "THIS IS A CHEST GAME" << endl;


	wcout << "Made by Usin" << '\t' << "Version 1.0.0" << endl << endl;
	
	//CONSOLE_FONT_INFOEX cfi;
	//cfi.cbSize = sizeof(cfi);
	//cfi.nFont = 0;
	//cfi.dwFontSize.X = 0;                   // Width of each character in the font
	//cfi.dwFontSize.Y = 24;                  // Height
	//cfi.FontFamily = FF_DONTCARE;
	//cfi.FontWeight = FW_NORMAL;
	//wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	//SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	SetConsoleTextAttribute(outcon, CL_WHITE);
	wcout << "New game" << endl;							// X <= 7, Y == 3
	wcout << "Continue game" << endl;					// X <= 12, Y == 4
	wcout << "How to play" << endl;						// X <= 10, Y == 5
//	cout << "Quit game" << endl;
	return wcout << "Quit game" << endl;;				// X <= 8, Y == 6
}

GameSystem::GameSystem(int _scene)
{
	scene = _scene;
}

/*
	Return the scene of the gamesystem.
*/
int GameSystem::GetScene() const
{
	return scene;
}

/*
	Set scene of the gamesystem.
	Scene change will lead to game change.
	This is based on what I leart from Unity engine. Control game state by control game scene
*/
void GameSystem::SetScene(int newScene)
{
	scene = newScene;
}

/*
	Print out the menu, as well as handle mouse click in GetMouseInput.
	Depend on the return from mouse click, set the game state correct with user input.

*/
void PlayMenu(GameSystem &gamesystem) {
	system("CLS"); // clear console screen;
	wcout << gamesystem;
	bool isExit = false;
	while (!isExit) {
		pair<int, int> cord(GetMouseInput());
		int xcord = cord.first;
		int ycord = cord.second;

		if ((xcord <= 7) && (ycord == NEWGAME_YCORD)) {
			gamesystem.SetScene(NEWSCENE);
			isExit = true;
		}

		if ((xcord <= 12) && (ycord == CONTINUE_YCORD)) {
			gamesystem.SetScene(LOADSCENE);
			isExit = true;
		}

		if ((xcord <= 12) && (ycord == HOWTOPLAY_YCORD)) {
			gamesystem.SetScene(HOWTOPLAYSCENE);
			isExit = true;
		}

		if ((xcord <= 8) && (ycord == QUIT_YCORD)) {
			gamesystem.SetScene(ENDSCENE);
			isExit = true;
		}
	}
}

/*
	Print out the How to play game.

	When you change stage from different font size, the smaller font size won't display correctly.
	A "quick" fix is to windowed the console and then go full screen again.

	Then if the user click on MENU then return to menu
*/
void PlayHowToPlay(GameSystem &gamesystem) {	
	system("CLS"); // clear console screen;

	HANDLE outcon = GetStdHandle(STD_OUTPUT_HANDLE);		//you don't have to call this function every time
	CONSOLE_FONT_INFOEX font;								//CONSOLE_FONT_INFOEX is defined in some windows header	
	GetCurrentConsoleFontEx(outcon, false, &font);			//PCONSOLE_FONT_INFOEX is the same as CONSOLE_FONT_INFOEX*
	font.cbSize = sizeof(font);								//Need to set cbSize
	font.dwFontSize.X = 0;
	font.dwFontSize.Y = FZ_HOWTOPLAY;
	font.FontWeight = FW_NORMAL;
	wcscpy_s(font.FaceName, L"MS Gothic"); // Choose your font
	SetCurrentConsoleFontEx(outcon, false, &font);
	SetConsoleTextAttribute(outcon, CL_YELLOW);

	//When you change stage from different font size, the smaller font size won't display correctly.
	//A "quick" fix is to windowed the console and then go full screen again.
	SetConsoleDisplayMode(outcon, CONSOLE_WINDOWED_MODE, 0); // FULL SCREEN
	SetConsoleDisplayMode(outcon, CONSOLE_FULLSCREEN_MODE, 0); // FULL SCREEN


	wcout << "A C++ project at Metropolia UAS" << endl;
	wcout << "Finland, Jan 2018" << endl << endl;

	SetConsoleTextAttribute(outcon, CL_WHITE);

	wcout << "This game only use mouse to select and move the chest piece." << endl << endl;
	wcout << "Click NEW GAME to start a new game." << endl << endl;
	wcout << "To cancel a chest move, click on the chest piece again, " << endl << "or outside the board." << endl << endl;
	wcout << "While playing the game, click SAVE GAME to save game." << endl;
	wcout << "SAVE GAME won't work when player are moving the chest piece." << endl << endl;
	wcout << "This game need at least two people, or a person with" << endl << "multiple personality disorder to play." << endl << endl;

	SetConsoleTextAttribute(outcon, CL_YELLOW);

	wcout << "GAME MENU";
	bool isExit = false;
	while (!isExit) {
		pair<int, int> cord(GetMouseInput());
		int xcord = cord.first;
		int ycord = cord.second;

		if ((xcord <= 8) && (ycord == 16)) {
			gamesystem.SetScene(MENUSCENE);
			isExit = true;
		}
	}
}


/*
	Get Mouse input from the user.
	Each mouse input read will return 1 pair int, int. The first int is the column, then second int is the row
	Notice that XCORD = COLUMN and YCORD = ROW

	For wide character, 1 wide character = 2 normal character, so when convert to COLUMN from XCORD, we need to divide by 2

	Notice that to handle mouse input, we need to disable 'Quick Edit Mode', which require the ENABLE_EXTENDED_FLAGS

	For now, the game don't handle any input aside from mouse, so we can ignore any other input. And by ignore I mean
	to put break; in those case. Since I also handle stranger input with error message.
*/
pair<int, int> GetMouseInput()
{
	HANDLE hStdin;
	DWORD fdwSaveOldMode, cNumRead, fdwMode;
	INPUT_RECORD irInBuf;

	// Get the standard input handle.

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle", NULL, NULL);

	// Save the current input mode, to be restored on exit.

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode", hStdin, fdwSaveOldMode);

	// Step-1:Disable 'Quick Edit Mode' option using  ENABLE_EXTENDED_FLAGS
	// With "Quick Edit Mode' the console can't get the mouse event
	// https://stackoverflow.com/questions/42213161/console-mouse-input-not-working

	// Step-2:Enable the mouse input events, after you have already applied that 'ENABLE_EXTENDED_FLAGS'
	//to disable 'Quick Edit Mode'

	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode", hStdin, fdwSaveOldMode);

	bool isExit = false;
	while (!isExit) {
		if (!ReadConsoleInput(
			hStdin,			// input buffer handle
			&irInBuf,		// buffer to read into
			1,				// size of read buffer
			&cNumRead))		// number of records read
			ErrorExit("ReadConsoleInput", hStdin, fdwSaveOldMode);

		// Dispatch the events to the appropriate handler.

		switch (irInBuf.EventType)
		{

		case MOUSE_EVENT: // mouse input
						  //MouseEventProc(irInBuf.Event.MouseEvent);
			if (irInBuf.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				int xcord = irInBuf.Event.MouseEvent.dwMousePosition.X;
				int ycord = irInBuf.Event.MouseEvent.dwMousePosition.Y;
				// Restore input mode on exit.
				SetConsoleMode(hStdin, fdwSaveOldMode);
				return pair<int, int> (xcord, ycord);
			}
			break;

		case KEY_EVENT:					// disregard key events
		case WINDOW_BUFFER_SIZE_EVENT:	// disregard focus events
		case FOCUS_EVENT:				// disregard focus events
		case MENU_EVENT:				// disregard menu events
			break;

		default:
			//ErrorExit("Unknown event type", hStdin, fdwSaveOldMode);
			break;
		}		
	}	
}


/*
	Handle error from other input
	NOT SURE IF THIS IS NEEDED.
*/
void ErrorExit(LPSTR lpszMessage, HANDLE hStdin, DWORD fdwSaveOldMode)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	//exit(EXIT_FAILURE);										// Should not use exit() to exit.
}


