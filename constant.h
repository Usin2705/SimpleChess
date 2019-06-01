#pragma once

#include "windows.h"					

using namespace std;

#define FZ_HOWTOPLAY 40						// font size
#define FZ_BOARD 80						// font size
#define FZ_OPTION 80					// font size

// COLOR code for SetConsoleTextAttribute()
const WORD CL_YELLOW = 14;
const WORD CL_WHITE = 15;
//color = 0->15; background = color * 16
const WORD CL_BLACK_DARKBACKGROUND = 0 + 8*16;
const WORD CL_BLACK_LIGHTBACKGROUND = 0 + CL_WHITE *16;

const int MENUSCENE = 0;
const int NEWSCENE = 1;
const int LOADSCENE = 2;
const int HOWTOPLAYSCENE = 3;
const int ENDSCENE = 4;

const int NEWGAME_YCORD = 3;
const int CONTINUE_YCORD = 4;
const int HOWTOPLAY_YCORD = 5;
const int QUIT_YCORD = 6;


const int TURN_GAME_BOARD_YCORD = 0;
const int SAVE_GAME_BOARD_YCORD = 2;
const int MENU_GAME_BOARD_YCORD = 5;


const int BOARD_SIZE = 8;

// Chest color
const int WHITE = 0;
const int BLACK = 1;
const int EMPTY = 2;


const string SAVEFILE = "gamesave.txt";

// chest wchar
const wchar_t WK = L'\u2654';
const wchar_t WQ = L'\u2655';
const wchar_t WR = L'\u2656';
const wchar_t WB = L'\u2657';
const wchar_t WH = L'\u2658';
const wchar_t WP = L'\u2659';

const wchar_t BK = L'\u265A';
const wchar_t BQ = L'\u265B';
const wchar_t BR = L'\u265C';
const wchar_t BB = L'\u265D';
const wchar_t BH = L'\u265E';
const wchar_t BP = L'\u265F';

const wchar_t ET = L'\u3000';		// The width of ideographic (CJK) characters.  http://jkorpela.fi/chars/spaces.html

									
// chest char
const char CWK = 'K';
const char CWQ = 'Q';
const char CWR = 'R';
const char CWB = 'B';
const char CWH = 'H';
const char CWP = 'P';

const char CBK = 'k';
const char CBQ = 'q';
const char CBR = 'r';
const char CBB = 'b';
const char CBH = 'h';
const char CBP = 'p';

const char CET = 'E';		