#pragma once

#include <vector>

#include "constants.h"
#include "utils.h"
#include "board.h"

class Board;

// A falling piece
class Piece {
private:
	int locx;
	int locy;
	int rotationFrame;
	Block pieceType;
	Block blockMap[PIECE_SIZE][PIECE_SIZE];

public:
	Piece(Block blockInitial);

	// Return whether the piece is in a valid position
	bool isValid(Board& board);

	// Translate the piece by the specified deltas
	// Deltas should be in the range -1..=1, and only one should be nonzero
	// Return whether the piece should be locked in
	bool move(int deltax, int deltay, Board& board);

	// Rotate the piece in the specified direction
	void rotate(bool clockwise, Board& board);

	// Get the piece's x location
	int getLocx();

	// Get the piece's y location
	int getLocy();

	// Get the cell at a local coordinate
	Block getCell(int x, int y);

	// Update the piece's block map based on its type and rotation frame
	void updateBlockMap();

	// Respawn the piece with a new type
	void respawn(Block newType);
};
