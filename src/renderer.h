#pragma once

#include <SFML/Graphics.hpp>

// Render the world
class Renderer {
private:
	sf::RenderWindow& window;

public:
	// Create the renderer
	Renderer(sf::RenderWindow& window);

	// Render the entire screen
	void renderGame();
};
