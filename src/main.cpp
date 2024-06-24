#include <SFML/Graphics.hpp>
#include <iostream>

#include "constants.h"
#include "utils.h"
#include "board.h"
#include "piece.h"
#include "renderer.h"
#include "bag.h"
#include "inputhandler.h"

/* TODO:
	- does build work on Linux or Mac?
	- implement the whole game: clearing lines, score and points, tspins
	- smarter rotation checking/locking
	- input refactor: only allow one rotation, store key press/release without deleting, arr/das, etc.
	- other?
*/

// Entry point
int main() {
	// TODO: save window position, allow resizing in the menu
	auto window = sf::RenderWindow{ { DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT }, "Supertris" };
	// TODO: better framerate limit ?
	window.setFramerateLimit(144);

	// Create game objects
	Renderer renderer(window);
	Board board;
	Bag bag;
	Piece piece(bag.popNextPiece());
	InputHandler inputHandler;
	// TODO: timing (calculate ms elapsed better) !

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
				inputHandler.press(event.key.scancode);
				break;
			case sf::Event::KeyReleased:
				// Key released
				inputHandler.release(event.key.scancode);
				break;
			}
		}
		// TODO: more updates (falling)
		// Update input
		int msElapsed = 10; // TODO: calibrate with clock
		inputHandler.updateCooldowns(msElapsed);
		// Process input
		if (inputHandler.isActive(sf::Keyboard::Scan::A)) {
			// Left
			// TODO: issue pressing both left and right at the same time (prioritize whichever was pressed latest)
			if (inputHandler.inCooldownData(sf::Keyboard::Scan::A)) {
				inputHandler.addToCooldown(sf::Keyboard::Scan::A, 20); // TODO: ARR
			} else {
				inputHandler.addToCooldown(sf::Keyboard::Scan::A, 140); // TODO: DAS
			}
			piece.move(-1, 0, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::D)) {
			// Right
			if (inputHandler.inCooldownData(sf::Keyboard::Scan::D)) {
				inputHandler.addToCooldown(sf::Keyboard::Scan::D, 20); // TODO: ARR
			} else {
				inputHandler.addToCooldown(sf::Keyboard::Scan::D, 140); // TODO: DAS
			}
			piece.move(1, 0, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::H)) {
			// Rotate left
			inputHandler.addToCooldown(sf::Keyboard::Scan::H);
			piece.rotate(false, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::L)) {
			// Rotate right
			inputHandler.addToCooldown(sf::Keyboard::Scan::L);
			piece.rotate(true, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::W)) {
			// Soft drop
			inputHandler.addToCooldown(sf::Keyboard::Scan::W, 40); // TODO: refactor into SDF
			bool shouldLock = piece.move(0, 1, board);
			if (shouldLock) {
				// TODO: Lock and reset
				int cleared = board.lockPiece(piece);
				piece.respawn(bag.popNextPiece());
			}
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::S)) {
			// Hard drop
			inputHandler.addToCooldown(sf::Keyboard::Scan::S);
			for (int i = 0; i < 20; i++) {
				piece.move(0, 1, board);
			}
			// TODO: Lock and reset
			int cleared = board.lockPiece(piece);
			piece.respawn(bag.popNextPiece());
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::R)) {
			// Restart
			inputHandler.addToCooldown(sf::Keyboard::Scan::R);
			board.reset();
			// TODO: also reset bag, piece, etc.
		}

		// TODO: more updates (falling)
		// Render
		renderer.renderGame(board, piece);
	}
	return 0;
}
