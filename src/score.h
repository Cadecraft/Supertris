#include "constants.h"
#include "utils.h"

// Store the score for the player
struct Score {
public:
	int points;
	int quads;
	int tspins;

	Score() {
		points = 0;
		quads = 0;
		tspins = 0;
	}
};
