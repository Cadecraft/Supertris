#pragma once

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
	// TODO: also store combos
	// TODO: also store all clears
	uint64_t startTime;
	uint64_t timeElapsed;
	bool dead;

	// Initialize the score with the time the game started
	Score(uint64_t startTimeMs) {
		points = 0;
		quads = 0;
		lines = 0;
		tspins = 0;
		currStreak = 0;
		startTime = startTimeMs;
		timeElapsed = 0;
		dead = false;
	}

	// Die
	void die() {
		dead = true;
	}

	// Update the score's internal timing
	void updateTime(uint64_t currentTime) {
		if (!dead) {
			timeElapsed = currentTime - startTime;
		}
	}

	// Get the number of milliseconds per fall based on the level (lines cleared)
	uint64_t getMsPerFall() {
		int level = lines / 10;
		return floor(3000 / (level + 3));
	}

	// Get the number of milliseconds before an auto-dropping piece auto-locks
	uint64_t getMsPerAutolock() {
		// Gives 1 second of freedom
		return 1000;
	}

	// Increase the score after a piece is locked based on score and game factors
	void increase(int linesCleared, bool wasTspin, bool isBoardClear) {
		// Increase for the piece lock
		points += 2;
		// Increase score and account for lines cleared
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
		// Increase for other conditions
		if (isBoardClear) {
			// All clear
			points += 100;
		}
	}
};
