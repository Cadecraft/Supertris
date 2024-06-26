#include "constants.h"
#include "utils.h"

// Store the score for the player
struct Score {
public:
	int points;
	int quads;
	int lines;
	int tspins;
	int currStreak;
	uint64_t startTime;
	uint64_t timeElapsed;

	Score(uint64_t startTimeMs) {
		points = 0;
		quads = 0;
		lines = 0;
		tspins = 0;
		currStreak = 0;
		startTime = startTimeMs;
	}

	void updateTime(uint64_t currentTime) {
		timeElapsed = currentTime - startTime;
	}

	void increase(int linesCleared, bool wasTspin) {
		// Increase for the piece lock
		points += 2;
		// Increase for lines cleared
		lines += linesCleared;
		switch (linesCleared) {
		case 0:
			// None: no points for lines cleared
			break;
		case 1:
			// Single
			points += 10;
			currStreak = 0;
			break;
		case 2:
			if (wasTspin) {
				// Tspin double
				points += 70;
				points += currStreak * 5;
				currStreak++;
				tspins++;
			} else {
				// No tspin double
				points += 25;
				currStreak = 0;
			}
			break;
		case 3:
			if (wasTspin) {
				// Tspin triple
				points += 85;
				points += currStreak * 5;
				currStreak++;
				tspins++;
			} else {
				// No tspin double
				points += 45;
				currStreak = 0;
			}
			break;
		case 4:
			// Quad
			points += 70;
			points += currStreak * 5;
			currStreak++;
			quads++;
			break;
		default:
			// Nothing else should happen
			break;
		}
	}
};
