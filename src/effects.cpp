#include "effects.h"

void Effects::resetByBoard(Board& board) {
	// Make the vector match
	sparkleBoard.clear();
	for (int y = 0; y < board.getHeight(); y++) {
		sparkleBoard.push_back({});
		for (int x = 0; x < board.getWidth(); x++) {
			sparkleBoard.back().push_back({});
		}
	}
}

void Effects::updateByFrame() {
	// Update each sparkle value to be slightly less
	for (int y = 0; y < sparkleBoard.size(); y++) {
		for (int x = 0; x < sparkleBoard[y].size(); x++) {
			if (sparkleBoard[y][x] > 0) sparkleBoard[y][x] -= 5;
			else sparkleBoard[y][x] = 0;
		}
	}
	// Update the beam to be slightly less
	beamStrength -= 5;
	if (beamStrength <= 0) {
		beamLocy = -1;
		beamStrength = 0;
	}
	flashStrength -= 2;
	if (flashStrength <= 0) {
		flashStrength = 0;
	}
}

int Effects::getSparkleValue(int x, int y) {
	if (x < 0 || y < 0 || x >= sparkleBoard[0].size() || y >= sparkleBoard.size()) return 0;
	return sparkleBoard[y][x];
}

void Effects::setSparkleValue(int x, int y, int val) {
	if (x < 0 || y < 0 || x >= sparkleBoard[0].size() || y >= sparkleBoard.size()) return;
	sparkleBoard[y][x] = val;
}

void Effects::sparklePiece(Piece& piece) {
	for (int y = 0; y < PIECE_SIZE; y++) {
		for (int x = 0; x < PIECE_SIZE; x++) {
			if (piece.getCell(x, y) != Block::None) {
				setSparkleValue(x + piece.getLocx(), y + piece.getLocy(), 100);
			}
		}
	}
}

void Effects::spawnFlash(Color color, int strength) {
	flashColor = color;
	flashStrength = strength;
}

void Effects::spawnBeam(int locy) {
	beamLocy = locy;
	beamStrength = 100;
}

int Effects::getBeamLocy() {
	return beamLocy;
}

int Effects::getBeamStrength() {
	return beamStrength;
}

Color Effects::getFlashColor() {
	return flashColor;
}

int Effects::getFlashStrength() {
	return flashStrength;
}
