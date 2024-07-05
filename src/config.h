#pragma once

#include "constants.h"
#include "utils.h"

// Store the user configuration for the game
class Config {
private:
	int arr; // Auto repeat rate, in milliseconds
	int das; // Delay auto shift, in milliseconds
	int sdf; // Soft drop factor, in milliseconds

public:
	// Create the default config
	Config() {
		// WASD: arr = 2, das = 140, sdf = 10
		arr = 2;
		das = 140;
		sdf = 10;
	}

	// Get the auto repeat rate in milliseconds
	int getArr() {
		return arr;
	}

	// Get the delay auto shift in milliseconds
	int getDas() {
		return das;
	}

	// Get the soft drop factor in milliseconds
	int getSdf() {
		return sdf;
	}
};
