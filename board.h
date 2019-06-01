// Manage the board of chest game
// It contain all important method for the chest game, including save, load board game.
// It also responsible to check the valid moves of the chest pieces.
// It only have two attribute, the number of turn passed, and the chestboard (which chests in on board)
#pragma once

#include <vector> 
#include "chest.h"
#include <fstream>
#include <iterator>     // std::ostream_iterator 
#include "gamesystem.h" // <iostream> "constant.h"

class Board {
	friend wostream& operator<<(wostream& out, const Board& board);
public:
	Board();
	void ChangeTurn();
	bool IsValidMove(int row, int column, int newRow, int newColumn, Chest chest) const;
	int SaveBoard() const;
	void LoadBoard(GameSystem &gamesystem);
	void LoadNewGame();
	void CheckValidMovesHorse(int row, int column, int chestColor, vector<pair<int, int>> &validMoves) const;
	void CheckValidMovesBishop(int row, int column, int chestColor, vector<pair<int, int>> &validMoves) const;
	void CheckValidMovesRook(int row, int column, int chestColor, vector<pair<int, int>> &validMoves) const;
	void CheckValidMovesKing(int row, int column, int chestColor, vector<pair<int, int>> &validMoves) const;
	bool CheckKing();
	int GetTurn() const;
	int MoveChest(int xcord, int ycord);
	Chest GetChest(int row, int column) const;
private:
	vector<vector<Chest>> chestboard;
	int turnNo;
};

/*
Get user mouse input
Then check if user select chestboard --> which piece --> whose turn is it --> ok then start to move.
If user select menu on the chest board (SAVE, RETURN TO MENU) then change game scene and start that option.

The board always check if the king was killed right after every move --> to finish the game.
*/
void PlayBoard(Board &board, GameSystem &gamesystem);