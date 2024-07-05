#pragma once

#include <vector>

#include "constants.h"
#include "utils.h"
#include "board.h"
#include "piece.h"

// Store the frame-by-frame effects to connect the game to the rendering system
class Effects {
private:
	std::vector<std::vector<int>> sparkleBoard; // Sparkle brightness from 0 to 100

public:
	// Reset the effects, conforming to the board's current shape
	void resetByBoard(Board& board);

	// Update the effects (call once every frame)
	void updateByFrame();

	// Get the sparkle value at a specific cell from 0 to 100
	int getSparkleValue(int x, int y);

	// Set the sparkle value at a specific cell from 0 to 100
	void setSparkleValue(int x, int y, int val);

	// Set the sparkle value to match a piece's location
	void sparklePiece(Piece& piece);
};
