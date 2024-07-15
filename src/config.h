#pragma once

#include <iostream>
#include <fstream>
#include <string>

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
        // Pro: arr = 0, das = 140, sdf = 0
		arr = 0; // 2
		das = 140;
		sdf = 0; // 10
        bool loadSuccess = load(CONFIG_FILENAME);
        bool saveSuccess = save(CONFIG_FILENAME);
	}

	// Save the config to a file
	bool save(std::string filename) {
		// Generate contents
		std::string contents = "";
		contents += "arr = ";
		contents += std::to_string(arr);
		contents += ";\n";
		contents += "das = ";
		contents += std::to_string(das);
		contents += ";\n";
		contents += "sdf = ";
		contents += std::to_string(sdf);
		contents += ";\n";
		// Write
		// TODO: better error handling
		try {
			std::ofstream file;
			file.open(filename);
			file << contents;
			file.close();
			return true;
		} catch (std::exception& e) {
			// TODO: handle
			return false;
		}
	}

	// Load the config to a file
	bool load(std::string filename) {
		// TODO: impl
		// Read and parse
		std::string contents = "";
		// TODO: better error handling
		try {
			std::ifstream file;
			file.open(filename);
			// Parse
			std::string currLine = "";
			// TODO: better delimiter?
			while (std::getline(file, currLine, ';')) {
				// Parse the current line
				std::string key = "";
				std::string value = "";
				bool foundEquals = false;
				for (char c : currLine) {
					if (c == '=') foundEquals = true;
					else if (c == ' ' || c == '\n' || c == '\r') continue;
					else if (foundEquals) value += c;
					else key += c;
				}
				// Update by the key and value
				if (key == "arr") {
					arr = 0;
					for (char c : value) {
						if (c >= '0' && c <= '9') {
							arr *= 10;
							arr += (c - '0');
						}
					}
				}
				if (key == "das") {
					das = 0;
					for (char c : value) {
						if (c >= '0' && c <= '9') {
							das *= 10;
							das += (c - '0');
						}
					}
				}
				if (key == "sdf") {
					sdf = 0;
					for (char c : value) {
						if (c >= '0' && c <= '9') {
							sdf *= 10;
							sdf += (c - '0');
						}
					}
				}
			}
			file.close();
			return true;
		} catch (std::exception& e) {
			// TODO: handle
			return false;
		}
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
