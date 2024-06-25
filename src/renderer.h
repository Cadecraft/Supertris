#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "constants.h"
#include "utils.h"
#include "bag.h"
#include "board.h"
#include "piece.h"

// Render the world
class Renderer {
private:
	sf::RenderWindow& window;
	int blockWidth;

public:
	// Create the renderer
	Renderer(sf::RenderWindow& window);

	// Render a rectangle onto the screen
	void renderRect(int x, int y, int width, int height, Color color);

	// Render the entire screen
	void renderGame(Board& board, Piece& piece, Bag& bag);
};
