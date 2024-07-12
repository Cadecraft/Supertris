#pragma once

#include <vector>
#include <deque>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "utils.h"

// Handles and generates the queue of upcoming pieces
class Bag {
private:
	// The back is the farthest-away piece; the front is the next one
	// Stores from one to two bags' worth of pieces (for smoothness)
	std::vector<Block> allBlocks;
	std::deque<Block> nextPieces;

	// Add the next bag of 7 pieces to the queue
	void addNextBag() {
		// Shuffle the 7 possible
		for (int i = 0; i < allBlocks.size(); i++) {
			int randidx = std::rand() % (i + 1);
			std::swap(allBlocks[i], allBlocks[randidx]);
		}
		// Add all 7 in the new random order
		for (Block b : allBlocks) {
			nextPieces.push_back(b);
		}
	}

public:
	Bag() {
		std::srand(time(NULL));
		// Initialize all possible blocks
		allBlocks = {
			Block::Z,
			Block::T,
			Block::S,
			Block::O,
			Block::L,
			Block::J,
			Block::I
		};
		// Initialize the next 2 bags' worth of pieces
		reset();
	}

	// Reset the bag
	void reset() {
		nextPieces.clear();
		addNextBag();
		addNextBag();
	}

	// Get and remove the next piece from the queue, filling in the next bag if needed
	Block popNextPiece() {
		if (nextPieces.empty()) {
			return Block::None;
		}
		Block res = nextPieces.front();
		nextPieces.pop_front();
		// If the queue is too small, add the next bag
		if (nextPieces.size() <= 7) {
			addNextBag();
		}
		return res;
	}

	// View the next n pieces in the queue (front is the next one)
	std::vector<Block> viewFuture(int n) {
		if (n >= nextPieces.size()) return { };
		std::vector<Block> res;
		for (int i = 0; i < n; i++) {
			res.push_back(nextPieces[i]);
		}
		return res;
	}
};
