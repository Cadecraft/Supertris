#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

// Handle all assets for the game
class AssetHandler {
private:
	sf::Font font;
	bool fontLoaded;

public:
	AssetHandler() {
		if (!font.loadFromFile("assets/LiberationSans-Regular.ttf")) {
			// Error
			std::cout << "ERROR: Could not load the font" << std::endl;
			fontLoaded = false;
		}
		fontLoaded = true;
	}

	// Get the font
	sf::Font& getFont() {
		return font;
	}
};
