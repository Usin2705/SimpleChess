#include "stdafx.h"
#include "board.h"
#include <io.h>
#include <fcntl.h>


/*
	Just an empty constructor. This in for saving processing time.
	When load chest game, chest piece will be push_back to an empty vector.
	When start new game, board will be creadted from LoadNewGame method.
	So if we make a 8x8 2D vector here it will be either override by load or new.
*/
Board::Board()
{
	
}

/*
	Fill the empty chestboard with new game.
	Remember to resize and set turn to 0 as user may load a game, then (without quit the program) start a new game. We are using push
		back so the old chestboard will not be overwrited.

*/
void Board::LoadNewGame()
{
	chestboard.resize(0);
	chestboard.push_back(vector<Chest> {BR, BH, BB, BQ, BK, BB, BH, BR});
	chestboard.push_back(vector<Chest> {BP, BP, BP, BP, BP, BP, BP, BP});
	chestboard.resize(6, vector<Chest> {ET, ET, ET, ET, ET, ET, ET, ET});
	chestboard.push_back(vector<Chest> {WP, WP, WP, WP, WP, WP, WP, WP});
	chestboard.push_back(vector<Chest> {WR, WH, WB, WQ, WK, WB, WH, WR});
	turnNo = 1;
}

/*
	While the name is change turn (to indicate the turn change from white <-> black, it also increase the number of turn by one.
	it is to keep track the current turn have passed in the game.
*/
void Board::ChangeTurn()
{
	turnNo++;
}


/*
	Return the number of turn passed in game, in int;
*/
int Board::GetTurn() const
{
	return turnNo;
}

/*
	This is for print out the board.
	We looped throught the 2D chestboard vector and print out any pieces on the board.

	For now, I'm using [] to loop, which is kind of easier. MAY CONSIDER USING FOR_EACH IN FUTURE IF I HAVE TIME.

	For the board, each adjacent cell have different color, so we need to change the output color each cell.
*/
wostream& operator<<(wostream &out, const Board &board)
{
	HANDLE outcon = GetStdHandle(STD_OUTPUT_HANDLE);		//you don't have to call this function every time
	CONSOLE_FONT_INFOEX font;								//CONSOLE_FONT_INFOEX is defined in some windows header	
	GetCurrentConsoleFontEx(outcon, false, &font);			//PCONSOLE_FONT_INFOEX is the same as CONSOLE_FONT_INFOEX*
	font.cbSize = sizeof(font);								//Need to set cbSize
	font.dwFontSize.X = 0;
	font.dwFontSize.Y = FZ_BOARD;
	font.FontWeight = FW_NORMAL;
	wcscpy_s(font.FaceName, L"MS Gothic");					// Choose your font. Only some fonts can display chestpiece in unicode
	SetCurrentConsoleFontEx(outcon, false, &font);
	SetConsoleTextAttribute(outcon, CL_WHITE);

	_setmode(_fileno(stdout), _O_U16TEXT);
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {

			//For the board, each adjacent cell have different color, so we need to change the output color each cell.
			if (i % 2 == 0) {
				j % 2 == 0 ? SetConsoleTextAttribute(outcon, CL_BLACK_DARKBACKGROUND) :
					SetConsoleTextAttribute(outcon, CL_BLACK_LIGHTBACKGROUND);
			}
			// The next row will have reverse color rule:
			else {
				j % 2 == 0 ? SetConsoleTextAttribute(outcon, CL_BLACK_LIGHTBACKGROUND) :
					SetConsoleTextAttribute(outcon, CL_BLACK_DARKBACKGROUND);
			}
			wcout << board.chestboard[i][j];
		}

		// print out the TURN of the game on the left of the board.
		if (i == TURN_GAME_BOARD_YCORD) {
			SetConsoleTextAttribute(outcon, CL_WHITE);
			wcout << "    TURN: " << board.GetTurn();
		}

		// print out the SAVE GAME OPTION on the left of the board.
		if (i == SAVE_GAME_BOARD_YCORD) {
			SetConsoleTextAttribute(outcon, CL_YELLOW);
			wcout << "    SAVE GAME";
		}

		// print out the GAME MENU OPTION on the left of the board.
		if (i == MENU_GAME_BOARD_YCORD) {
			SetConsoleTextAttribute(outcon, CL_YELLOW);
			wcout << "    GAME MENU";
		}
		wcout << endl;
	}
	SetConsoleTextAttribute(outcon, CL_WHITE);
	return wcout;
}


/*
	Return the chest piece on the board.
*/
Chest Board::GetChest(int row, int column) const
{
	return chestboard[row][column];
}


/*
	Get user mouse input
	Then check the validity of that move by method IsValidMove.
	If it return true then move the chest piece (old position = empty, new position = chest piece) and return EXIT_SUCESS.
	If it can't move to that cell, then return EXIT_FAILURE

	EXIT_SUCCESS: Move successed.
	EXIT_FAILURE: Failed to move.
*/
int Board::MoveChest(int row, int column) {
	bool isExit = false;
	while (!isExit) {
		pair<int, int> cord(GetMouseInput());
		int newColumn = cord.first / 2;
		int newRow = cord.second;
		if ((newRow < (BOARD_SIZE)) && (newColumn < (BOARD_SIZE))) {
			Chest tempChest = chestboard[row][column];
			if (IsValidMove(row, column, newRow, newColumn, tempChest)) {
				chestboard[row][column] = ET;
				chestboard[newRow][newColumn] = tempChest;
				return EXIT_SUCCESS;
			}		
		}
		isExit = true;
	}
	return EXIT_FAILURE;
}


/*
	Get user mouse input
	Then check if user select chestboard --> which piece --> whose turn is it --> ok then start to move.
	If user select menu on the chest board (SAVE, RETURN TO MENU) then change game scene and start that option.

	The board always check if the king was killed right after every move --> to finish the game.
*/
void PlayBoard(Board &board, GameSystem &gamesystem) {
	system("CLS"); // clear console screen;
	wcout << board;

	int turnColor = board.GetTurn() % 2 == 1 ? WHITE : BLACK;
	wcout << ((turnColor == WHITE) ? "White's turn." : "Black's turn.");

	bool isExit = false;
	while (!isExit) {
		pair<int, int> cord(GetMouseInput());
		int column = cord.first / 2;
		int row = cord.second;

		if ((column < (BOARD_SIZE)) && (row < (BOARD_SIZE))) {
			if (!(board.GetChest(row, column) == ET) && (board.GetChest(row, column).GetColor() == turnColor)) {
				wcout << " Please move your piece." << endl;
				board.MoveChest(row, column) == EXIT_SUCCESS ? board.ChangeTurn() : NULL;
				isExit = true;
			}
		}
		// Need to use cord, since the letter only count at half a column

		// SAVE GAME
		else if ((cord.first >= 20) && (cord.first <= 28) && (row == SAVE_GAME_BOARD_YCORD)) {
			board.SaveBoard();
			wcout << "Game saved";
			isExit = true;
		}

		// RETURN TO MENU
		else if ((cord.first >= 20) && (cord.first <= 28) && (row == MENU_GAME_BOARD_YCORD)) {
			gamesystem.SetScene(MENUSCENE);						//	return to menu
			isExit = true;
		}


		// Always check if the king was killed right after every move --> to finish the game.
		if (board.CheckKing()) {
			wcout << (board.GetTurn() % 2 == 1 ? "BLACK WON " : "WHITE WON ");
			system("PAUSE");
			gamesystem.SetScene(MENUSCENE);						//	return to menu
			isExit = true;
		}
	}
}

/*
	Check if the WHITE/BLACK KING is both on the board.
	If we can't find either of them then the game end.

	return FALSE if game still continue
	return TRUE if game end.
	
	
	IF WE MAKE THIS METHOD CONST THEN ITERATOR WON'T WORK. WHY?
*/
bool Board::CheckKing() {
	vector<Chest>::iterator it;						// iterator find
	int count = 0;

	for (unsigned int i = 0; i < chestboard.size(); i++) {
		it = find(chestboard[i].begin(), chestboard[i].end(), WK);
		if (it != chestboard[i].end()) {
			count++;
			break;
		}
	}

	for (unsigned int i = 0; i < chestboard.size(); i++) {
		it = find(chestboard[i].begin(), chestboard[i].end(), BK);
		if (it != chestboard[i].end()) {
			count++;
			break;
		}
	}
	return count == 2 ? false : true;
}

/*
	Save the board to the SAVEFILE.
	Also save the number of turn to file.
	Don't need any delimiter since we only have exactly 8X8 cell.
	return EXIT_SUCCESS if succesfully saved.
	THERE IS NO EXIT_FAILURE SINCE I TOO LAZY TO DO.
*/
int Board::SaveBoard() const
{
	ofstream output_file(SAVEFILE);
	ostream_iterator<Chest> output_iterator(output_file);
	for (unsigned int i = 0; i < chestboard.size(); i++) {
		copy(chestboard.at(i).begin(), chestboard.at(i).end(), output_iterator);
		//output_file << '\n';									//new line for manual check
	}		
	output_file << turnNo;
	output_file.close();

	return EXIT_SUCCESS;
}

/*
	Load the board from file.

	If error (don't have save file) then return to main menu

	If the save file is corrupted then also return to main menu (THIS NEVER HAPPEN)
*/
void Board::LoadBoard(GameSystem &gamesystem) {
	chestboard.resize(0);
	ifstream loadFile(SAVEFILE);

	// If error (don't have save file) then return to main menu
	if (!loadFile.is_open())
	{
		wcout << "Error! Save file not found" << endl;
		system("PAUSE");
		gamesystem.SetScene(MENUSCENE);
	}
	try {
		for (unsigned int i = 0; i < BOARD_SIZE; i++) {
			vector<Chest> chestVector;

			for (unsigned int j = 0; j < BOARD_SIZE; j++) {
				Chest chest;
				loadFile >> chest;
				chestVector.push_back(chest);
			}
			chestboard.push_back(chestVector);
		}

		loadFile >> turnNo;
	}
	
	//If the save file is corrupted then also return to main menu
	catch (...) {
		wcout << "Error! Save file is corrupted" << endl;
		system("PAUSE");
		gamesystem.SetScene(MENUSCENE);

	}
}

/*
	Check the validity of move of the chest piece.
	PAWN are checked directly inside this method. Since White Pawn and Black Pawn are different to check.
	Every other chest pieces are checked via their own check method.

	HOW TO DO THE CHECK:
	First create an empty 8X8 Vector.
	then gradaully check their adjacent cell for valid move. Step by Step, start from the closest cell.
	For each valid move, we push_back to the vector.
	If the path is block (by same color chest piece, or out of board then we stop that path)

	Then we will finally have a 8X* vector contain all possible move of the chest piece.
	(THIS WAS DONE SO THAT WE CAN PAINT THE BOARD WITH ALL POSSIBLE MOVE -- NOT YET DONE (MAYBE NEVER))

	Then we find the target move cell with 8X8 Vector contain possible moves. If found --> can move, else can't

	For move out of board (out of vector range), instead of make several if/else cases, we using try catch error.
	If out of range then catch the error (but doing nothing)
	It's much more easier to implement. 

	For Queen valid moves, her moves is a collection of ROOK's valid moves AND BISHOP's valid moves.

	RETURN TRUE if it is valid move.
	RETURN FALSE if not valid move.
*/
bool Board::IsValidMove(int row, int column, int newRow, int newColumn, Chest chest) const
{
	vector<pair<int, int>> validMoves;
	vector<pair<int, int>>::iterator it;						// iterator find
	if (chest == WP) {
		if ((row == 6) && (chestboard[row - 1][column] == ET) && (chestboard[row - 2][column] == ET)) {
			validMoves.push_back(make_pair(row - 2, column));
		}
		if ((row != 0) && (chestboard[row - 1][column] == ET)) {
			validMoves.push_back(make_pair(row - 1, column));
		}

		// for the following moves, need to check the out of range
		// only at() will work witk try catch
		// NEED FURTHER STUDY, SINCE IT'S APPEAR THAT AT() IS NOT AS GOOD AS []
		try {
			if (chestboard.at(row - 1).at(column + 1).GetColor() == BLACK) {
				validMoves.push_back(make_pair(row - 1, column + 1));
			}
		}
		catch (const out_of_range& oor) {}

		try {
			if (chestboard.at(row - 1).at(column - 1).GetColor() == BLACK) {
				validMoves.push_back(make_pair(row - 1, column - 1));
			}
		}
		catch (const out_of_range& oor) {}
	}

	if (chest == BP) {
		if ((row == 1) && (chestboard[row + 1][column] == ET) && (chestboard[row + 2][column] == ET)) {
			validMoves.push_back(make_pair(row + 2, column));
		}
		if ((row != 7) && (chestboard[row + 1][column] == ET)) {
			validMoves.push_back(make_pair(row + 1, column));
		}

		// for the following moves, need to check the out of range
		// only at() will work witk try catch
		// NEED FURTHER STUDY, SINCE IT'S APPEAR THAT AT() IS NOT AS GOOD AS []
		try {
			if ((chestboard.at(row + 1).at(column + 1).GetColor() == WHITE)) {
				validMoves.push_back(make_pair(row + 1, column + 1));
			}
		}
		catch (const out_of_range& oor) {}

		try {
			if (chestboard.at(row + 1).at(column - 1).GetColor() == WHITE) {
				validMoves.push_back(make_pair(row + 1, column - 1));
			}
		}
		catch (const out_of_range& oor) {}
	}

	if (chest == WH) CheckValidMovesHorse(row, column, WHITE, validMoves);
	if (chest == BH) CheckValidMovesHorse(row, column, BLACK, validMoves);

	if (chest == WB) CheckValidMovesBishop(row, column, WHITE, validMoves);
	if (chest == BB) CheckValidMovesBishop(row, column, BLACK, validMoves);

	if (chest == WR) CheckValidMovesRook(row, column, WHITE, validMoves);
	if (chest == BR) CheckValidMovesRook(row, column, BLACK, validMoves);


	// For Queen valid moves, her moves is a collection of ROOK's valid moves AND BISHOP's valid moves.
	if (chest == WQ) {
		CheckValidMovesBishop(row, column, WHITE, validMoves);
		CheckValidMovesRook(row, column, WHITE, validMoves);
	}

	if (chest == BQ) {
		CheckValidMovesBishop(row, column, BLACK, validMoves);
		CheckValidMovesRook(row, column, BLACK, validMoves);
	}

	if (chest == WK) CheckValidMovesKing(row, column, WHITE, validMoves);
	if (chest == BK) CheckValidMovesKing(row, column, BLACK, validMoves);

	it = find(validMoves.begin(), validMoves.end(), make_pair(newRow, newColumn));
	return it != validMoves.end() ? true : false;
}

/*
	Check all the valid moves of horse. (Knight)
	There're always 8 possible moves of the horse.
*/
void Board::CheckValidMovesHorse(int row, int column, int chestColor, vector<pair<int, int>> &validMoves) const
{
	// Horse possible move:
	// to the right: row +-1 column + 2
	// to the left: row +-1 column -2
	// to the top: row -2 column +-1
	// to the bottom: row +2 column +-1

	// for the following moves, need to check the out of range
	// only at() will work witk try catch
	// NEED FURTHER STUDY, SINCE IT'S APPEAR AT IS NOT AS GOOD AS []

	// to the right: row +-1 column + 2
	try {
		if (chestboard.at(row + 1).at(column + 2).GetColor() != chestColor) validMoves.push_back(make_pair(row + 1, column + 2));
	}
	catch (const out_of_range& oor) {}

	try {
		if (chestboard.at(row - 1).at(column + 2).GetColor() != chestColor) validMoves.push_back(make_pair(row - 1, column + 2));
	}
	catch (const out_of_range& oor) {}

	// to the left: row +-1 column -2
	try {
		if (chestboard.at(row + 1).at(column - 2).GetColor() != chestColor) validMoves.push_back(make_pair(row + 1, column - 2));
	}
	catch (const out_of_range& oor) {}

	try {
		if (chestboard.at(row - 1).at(column - 2).GetColor() != chestColor) validMoves.push_back(make_pair(row - 1, column - 2));
	}
	catch (const out_of_range& oor) {}

	// to the top: row -2 column +-1
	try {
		if (chestboard.at(row - 2).at(column + 1).GetColor() != chestColor) validMoves.push_back(make_pair(row - 2, column + 1));
	}
	catch (const out_of_range& oor) {}

	try {
		if (chestboard.at(row - 2).at(column - 1).GetColor() != chestColor) validMoves.push_back(make_pair(row - 2, column - 1));
	}
	catch (const out_of_range& oor) {}

	// to the bottom: row +2 column +-1
	try {
		if (chestboard.at(row + 2).at(column + 1).GetColor() != chestColor) validMoves.push_back(make_pair(row + 2, column + 1));
	}
	catch (const out_of_range& oor) {}

	try {
		if (chestboard.at(row + 2).at(column - 1).GetColor() != chestColor) validMoves.push_back(make_pair(row + 2, column - 1));
	}
	catch (const out_of_range& oor) {}
}

/*
	Check the valid moves of bishop.
	Bishop have 4 direction of movements
*/
void Board::CheckValidMovesBishop(int row, int column, int chestColor, vector<pair<int, int>>& validMoves) const
{
	// Move bottom right
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row + i).at(column + i).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row + i, column + i));
			}
			else if (chestboard.at(row + i).at(column + i).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row + i, column + i));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}

	// Move top right
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row - i).at(column + i).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row - i, column + i));
			}
			else if (chestboard.at(row - i).at(column + i).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row - i, column + i));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}

	// Move bottom left
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row + i).at(column - i).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row + i, column - i));
			}
			else if (chestboard.at(row + i).at(column - i).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row + i, column - i));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}

	// Move top right
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row - i).at(column - i).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row - i, column - i));
			}
			else if (chestboard.at(row - i).at(column - i).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row - i, column - i));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}

}

/*
Check the valid moves of rook.
Bishop have 4 direction of movement
*/
void Board::CheckValidMovesRook(int row, int column, int chestColor, vector<pair<int, int>>& validMoves) const
{
	// Move right
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row).at(column + i).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row, column + i));
			}
			else if (chestboard.at(row).at(column + i).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row, column + i));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}

	// Move left
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row).at(column - i).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row, column - i));
			}
			else if (chestboard.at(row).at(column - i).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row, column - i));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}

	// Move top
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row - i).at(column).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row - i, column));
			}
			else if (chestboard.at(row - i).at(column).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row - i, column));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}

	// Move bottom
	try {
		for (int i = 1; i < BOARD_SIZE; i++) {
			if (chestboard.at(row + i).at(column).GetColor() == EMPTY) {
				validMoves.push_back(make_pair(row + i, column));
			}
			else if (chestboard.at(row + i).at(column).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row + i, column));
				break;
			}
			else {
				break;
			}
		}
	}
	catch (const out_of_range& oor) {}
}


/*
	Check the valid moves of King.
	King have 8 possible valid moves.
*/
void Board::CheckValidMovesKing(int row, int column, int chestColor, vector<pair<int, int>>& validMoves) const
{
	// 1 Move right
	try {
		if (chestboard.at(row).at(column + 1).GetColor() != chestColor) {
				validMoves.push_back(make_pair(row, column + 1));
			}
	} catch (const out_of_range& oor) {}

	// 2 Move top right
	try {
		if (chestboard.at(row - 1).at(column + 1).GetColor() != chestColor) {
			validMoves.push_back(make_pair(row - 1, column + 1));
		}
	} catch (const out_of_range& oor) {}

	// 3 Move top
	try {
		if (chestboard.at(row - 1).at(column).GetColor() != chestColor) {
			validMoves.push_back(make_pair(row - 1, column));
		}
	} catch (const out_of_range& oor) {}

	// 4 Move top left
	try {
		if (chestboard.at(row - 1).at(column - 1).GetColor() != chestColor) {
			validMoves.push_back(make_pair(row - 1, column - 1));
		}
	} catch (const out_of_range& oor) {}

	// 5 Move left
	try {
		if (chestboard.at(row).at(column - 1).GetColor() != chestColor) {
			validMoves.push_back(make_pair(row, column - 1));
		}
	} catch (const out_of_range& oor) {}

	// 6 Move bottom left
	try {
		if (chestboard.at(row + 1).at(column - 1).GetColor() != chestColor) {
			validMoves.push_back(make_pair(row + 1, column - 1));
		}
	}catch (const out_of_range& oor) {}

	// 7 Move bottom 
	try {
		if (chestboard.at(row + 1).at(column).GetColor() != chestColor) {
			validMoves.push_back(make_pair(row + 1, column));
		}
	}catch (const out_of_range& oor) {}

	// 8 Move bottom right
	try {
		if (chestboard.at(row + 1).at(column + 1).GetColor() != chestColor) {
			validMoves.push_back(make_pair(row + 1, column + 1));
		}
	} catch (const out_of_range& oor) {}
}