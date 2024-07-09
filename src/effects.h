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
	int beamLocy = -1;
	int beamStrength = 0; // Horizontal beam brightness from 0 to 100
	Color flashColor = { 0, 0, 0 };
	int flashStrength = 0;

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

	// Set the beam (will appear directly below the blocks at locy)
	void spawnBeam(int locy);

	// Get the beam location y (beam should appear directly below these blocks)
	int getBeamLocy();

	// Get the beam strength from 0 to 100
	int getBeamStrength();

	// Spawn the screen flash of a certain color and strength from 0 to 100
	void spawnFlash(Color color, int strength);

	// Get the flash color
	Color getFlashColor();

	// Get the flash strength
	int getFlashStrength();
};
