#pragma once

#include <vector>

#include "constants.h"
#include "utils.h"
#include "piece.h"

class Piece;

// The board for the game
class Board {
private:
	int width;
	int height;
	std::vector<std::vector<Block>> board;

public:
	// Create a new board
	Board();

	int getWidth();
	int getHeight();
	// Return whether a coordinate is in bounds
	bool inBounds(int x, int y);
	// Get the block at a specific cell
	Block getCell(int x, int y);
	// DEBUG: Put a block at a specific cell
	void debug_putCell(int x, int y, Block toPut);
	// Lock a piece and return the number of rows cleared
	int lockPiece(Piece& piece);
	// Reset the board based on the width and height
	void reset();
	// Get whether the board is fully clear
	bool isClear();
};
