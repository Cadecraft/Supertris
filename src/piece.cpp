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

bool Piece::shouldLockNext(Board& board) {
	// Check below
	locy += 1;
	bool res = !isValid(board);
	locy -= 1;
	return res;
}

void Piece::rotate(int direction, Board& board) {
	int orig = rotationFrame;
	if (direction == 1) {
		rotationFrame++;
		if (rotationFrame >= 4) rotationFrame = 0;
	} else if (direction == -1) {
		rotationFrame--;
		if (rotationFrame < 0) rotationFrame = 3;
	} else {
		rotationFrame += 2;
		if (rotationFrame >= 4) rotationFrame -= 4;
	}
	updateBlockMap();
	if (!isValid(board)) {
		// Invalid: try to find a spot
		int origlocx = locx;
		int origlocy = locy;
		// trymatrix[i]: {x, y}
		const int tryMatrixSize = 8;
		// TODO: different trymatrices per piece/piece rotation frame?
		int tryMatrix[tryMatrixSize][2] = {
			{ -1, 0 },
			{ -1, 1 },
			{ -1, -1 },
			{ 0, 1 },
			{ 0, -1 },
			{ 1, 0 },
			{ 1, 1 },
			{ 1, -1 }
		};
		for (int i = 0; i < tryMatrixSize; i++) {
			locx = origlocx + tryMatrix[i][0];
			locy = origlocy + tryMatrix[i][1];
			if (isValid(board)) return;
		}
		// Still invalid: must undo
		rotationFrame = orig;
		locx = origlocx;
		locy = origlocy;
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

Block Piece::getPieceType() {
	return pieceType;
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
