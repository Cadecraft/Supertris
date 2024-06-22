#pragma once

#include <SFML/Graphics.hpp>

#include "constants.h"
#include "utils.h"
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

	// Render the entire screen
	void renderGame(Board& board, Piece& piece);
};
