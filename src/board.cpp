#include "board.h"

void Board::reset() {
	board.clear();
	for (int y = 0; y < height; y++) {
		board.push_back({});
		for (int x = 0; x < height; x++) {
			board.back().push_back(Block::None);
		}
	}
}

Board::Board() {
	width = INITIAL_BOARD_WIDTH;
	height = INITIAL_BOARD_HEIGHT;
	reset();
}

int Board::getWidth() {
	return width;
}

int Board::getHeight() {
	return height;
}

bool Board::inBounds(int x, int y) {
	return (x >= 0 && y >= 0 && x < width && y < height);
}

Block Board::getCell(int x, int y) {
	if (!inBounds(x, y)) {
		return Block::None;
	}
	return board[y][x];
}

void Board::debug_putCell(int x, int y, Block toPut) {
	if (!inBounds(x, y)) {
		return;
	}
	board[y][x] = toPut;
}

int Board::lockPiece(Piece& piece) {
	if (!piece.isValid(*this)) return 0; // Invalid piece cannot be locked
	for (int y = 0; y < PIECE_SIZE; y++) {
		for (int x = 0; x < PIECE_SIZE; x++) {
			if (piece.getCell(x, y) != Block::None) {
				// Place here
				int globalx = x + piece.getLocx();
				int globaly = y + piece.getLocy();
				board[globaly][globalx] = piece.getCell(x, y);
			}
		}
	}
	int rowsCleared = 0;
	// TODO: row clearing and count
	return rowsCleared;
}
