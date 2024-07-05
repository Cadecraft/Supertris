#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "constants.h"
#include "utils.h"
#include "bag.h"
#include "board.h"
#include "piece.h"
#include "assethandler.h"
#include "score.h"
#include "effects.h"

// Render the world
class Renderer {
private:
	sf::RenderWindow& window;
	AssetHandler& assetHandler;
	int blockWidth;

public:
	// Create the renderer
	Renderer(sf::RenderWindow& window, AssetHandler& assetHandler);

	// Render a rectangle onto the screen
	void renderRect(int x, int y, int width, int height, Color color);

	// Render text onto the screen
	void renderText(int x, int y, int size, Color color, sf::Font& font, std::string str);

	// Render a block onto the screen
	void renderBlock(int x, int y, int boardOffsetx, int boardOffsety, int widthToUse, Block cellHere, int sparkleValue);

	// Render the entire screen
	void renderGame(Board& board, Piece& piece, Bag& bag, Block holdBlock, Score& score, Effects& effects);
};
