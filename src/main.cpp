#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>

#include "constants.h"
#include "utils.h"
#include "board.h"
#include "piece.h"
#include "renderer.h"
#include "bag.h"

/* TODO:
	- does build work on Linux or Mac?
	- implement the whole game
	- other?
*/

// Entry point
int main() {
	// TODO: save window position, allow resizing in the menu
	auto window = sf::RenderWindow{ { 300u, 600u }, "Supertris" };
	// TODO: better framerate limit ?
	window.setFramerateLimit(144);

	// Create game objects
	Renderer renderer(window);
	Board board;
	Bag bag;
	Piece piece(bag.popNextPiece());
	// TODO: refactor keyboard into a separate input class
	std::set<sf::Keyboard::Scan::Scancode> keysPressed;

	// Main game loop
	while (window.isOpen()) {
		// Poll events
		for (auto event = sf::Event{}; window.pollEvent(event);) {
			switch (event.type) {
			case sf::Event::Closed:
				// Close window
				window.close();
				break;
			case sf::Event::KeyPressed:
				// Key pressed
				keysPressed.insert(event.key.scancode);
				break;
			case sf::Event::KeyReleased:
				// Key released
				keysPressed.erase(event.key.scancode);
				break;
			}
		}
		// TODO: more updates (falling)
		// TODO: input
		if (keysPressed.find(sf::Keyboard::Scan::A) != keysPressed.end()) {
			// Left
			keysPressed.erase(sf::Keyboard::Scan::A);
			piece.move(-1, 0, board);
			// TODO: DAS/ARR/etc.
		}
		if (keysPressed.find(sf::Keyboard::Scan::D) != keysPressed.end()) {
			// Right
			keysPressed.erase(sf::Keyboard::Scan::D);
			piece.move(true, 0, board);
		}
		if (keysPressed.find(sf::Keyboard::Scan::H) != keysPressed.end()) {
			// Rotate left
			keysPressed.erase(sf::Keyboard::Scan::H);
			piece.rotate(false, board);
			// TODO: DAS/ARR/etc.
		}
		if (keysPressed.find(sf::Keyboard::Scan::L) != keysPressed.end()) {
			// Rotate right
			keysPressed.erase(sf::Keyboard::Scan::L);
			piece.rotate(1, board);
		}
		if (keysPressed.find(sf::Keyboard::Scan::W) != keysPressed.end()) {
			// Soft drop
			keysPressed.erase(sf::Keyboard::Scan::W);
			bool shouldLock = piece.move(0, 1, board);
			if (shouldLock) {
				// TODO: Lock and reset
				board.lockPiece(piece);
				piece.respawn(bag.popNextPiece());
			}
		}
		if (keysPressed.find(sf::Keyboard::Scan::S) != keysPressed.end()) {
			// Hard drop
			keysPressed.erase(sf::Keyboard::Scan::S);
			for (int i = 0; i < 20; i++) {
				piece.move(0, 1, board);
			}
			// TODO: Lock and reset
			board.lockPiece(piece);
			piece.respawn(bag.popNextPiece());
		}
		if (keysPressed.find(sf::Keyboard::Scan::R) != keysPressed.end()) {
			// Restart
			keysPressed.erase(sf::Keyboard::Scan::R);
			board.reset();
			// TODO: also reset bag, piece, etc.
		}

		// TODO: rotation
		// TODO: more updates (falling)
		// Render
		renderer.renderGame(board, piece);
	}
	return 0;
}
