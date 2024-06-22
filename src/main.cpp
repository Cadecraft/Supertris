#include <SFML/Graphics.hpp>
#include <iostream>

#include "renderer.h"

/* TODO:
	- build not working on Linux?
	- implement the whole game
	- other?
*/

// Entry point
int main() {
	auto window = sf::RenderWindow{ { 300u, 600u }, "Supertris" };
	// TODO: better framerate limit ?
	window.setFramerateLimit(144);

	// Create game objects
	Renderer renderer(window);

	// Main game loop
	while (window.isOpen()) {
		// Poll events
		for (auto event = sf::Event{}; window.pollEvent(event);) {
			if (event.type == sf::Event::Closed) {
				// Close window
				window.close();
			}
		}
		// TODO: input
		// TODO: update
		// Render
		renderer.renderGame();
	}
	return 0;
}
