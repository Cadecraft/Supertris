#include "piece.h"

Piece::Piece(Block blockInitial) {
	// TODO: randomize the piece type to start
	respawn(blockInitial);
}

bool Piece::isValid(Board& board) {
	for (int y = 0; y < PIECE_SIZE; y++) {
		for (int x = 0; x < PIECE_SIZE; x++) {
			if (getCell(x, y) != Block::None) {
				// Consider this cell in the global space
				int globalx = x + locx;
				int globaly = y + locy;
				if (globalx < 0 || globaly < 0 || globalx >= board.getWidth() || globaly >= board.getHeight()) {
					// Out of bounds
					return false;
				}
				if (board.getCell(globalx, globaly) != Block::None) {
					// Overlapping block
					return false;
				}
			}
		}
	}
	// No invalid blocks
	return true;
}

bool Piece::move(int deltax, int deltay, Board& board) {
	// Change the position
	locx += deltax;
	locy += deltay;
	if (!isValid(board)) {
		// Invalid: must undo
		locx -= deltax;
		locy -= deltay;
		// Lock in if was moving down
		if (deltay == 1) return true;
	}
	return false;
}

void Piece::rotate(bool clockwise, Board& board) {
	int orig = rotationFrame;
	if (clockwise) {
		rotationFrame++;
		if (rotationFrame >= 4) rotationFrame = 0;
	} else {
		rotationFrame--;
		if (rotationFrame < 0) rotationFrame = 3;
	}
	updateBlockMap();
	if (!isValid(board)) {
		// Invalid: must undo
		rotationFrame = orig;
		updateBlockMap();
	}
}

int Piece::getLocx() {
	return locx;
}

int Piece::getLocy() {
	return locy;
}

Block Piece::getCell(int x, int y) {
	if (x < 0 || y < 0 || x >= PIECE_SIZE || y >= PIECE_SIZE) return Block::None;
	return blockMap[y][x];
}

void Piece::updateBlockMap() {
	if (pieceType == Block::None) return; // Impossible
	// Copy from the piece data
	for (int y = 0; y < PIECE_SIZE; y++) {
		for (int x = 0; x < PIECE_SIZE; x++) {
			blockMap[y][x] = numberToBlock(PIECE_DATA[blockToIndex(pieceType)][rotationFrame][y][x]);
		}
	}
}

void Piece::respawn(Block newType) {
	pieceType = newType;
	locx = 3; // TODO: correct coordinates?
	locy = 1;
	rotationFrame = 0;
	updateBlockMap();
}
