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
	uint64_t previousTime = currentTimeMs();

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
		// Get the number of millseconds elapsed
		int msElapsed = currentTimeMs() - previousTime; // TODO: check clock calibration
		previousTime = currentTimeMs();
		int frameRate = 1000.0 / msElapsed;
		std::cout << frameRate << " FPS" << std::endl;
		// TODO: more updates (falling)
		// Update input
		inputHandler.updateCooldowns(msElapsed);
		// Process input
		if (inputHandler.isActive(sf::Keyboard::Scan::A) && !inputHandler.inCooldownData(sf::Keyboard::Scan::D)) {
			// Left
			// TODO: issue pressing both left and right at the same time (prioritize whichever was pressed latest)
			if (inputHandler.inCooldownData(sf::Keyboard::Scan::A)) {
				inputHandler.addToCooldown(sf::Keyboard::Scan::A, 2); // TODO: ARR ("infinite" should just for(10))
			} else {
				inputHandler.addToCooldown(sf::Keyboard::Scan::A, 140); // TODO: DAS
			}
			piece.move(-1, 0, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::D) && !inputHandler.inCooldownData(sf::Keyboard::Scan::A)) {
			// Right
			if (inputHandler.inCooldownData(sf::Keyboard::Scan::D)) {
				inputHandler.addToCooldown(sf::Keyboard::Scan::D, 2); // TODO: ARR
			} else {
				inputHandler.addToCooldown(sf::Keyboard::Scan::D, 140); // TODO: DAS
			}
			piece.move(1, 0, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::Left)) {
			// Rotate left
			inputHandler.addToCooldown(sf::Keyboard::Scan::Left);
			piece.rotate(-1, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::Right)) {
			// Rotate right
			inputHandler.addToCooldown(sf::Keyboard::Scan::Right);
			piece.rotate(1, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::Up)) {
			// Rotate 180 degrees
			inputHandler.addToCooldown(sf::Keyboard::Scan::Up);
			piece.rotate(2, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::W)) {
			// Soft drop
			inputHandler.addToCooldown(sf::Keyboard::Scan::W, 10); // TODO: refactor into SDF
			bool shouldLock = piece.move(0, 1, board);
			shouldLock = false; // TODO: only lock if enough time has elapsed since the piece first hit the bottom
			// TODO: non-locking for a cooldown second if staying in the same spot
			if (shouldLock) {
				// Lock and reset
				int cleared = board.lockPiece(piece);
				piece.respawn(bag.popNextPiece());
				// TODO: scoring
			}
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::S)) {
			// Hard drop
			inputHandler.addToCooldown(sf::Keyboard::Scan::S);
			for (int i = 0; i < 20; i++) {
				piece.move(0, 1, board);
			}
			// Lock and reset
			int cleared = board.lockPiece(piece);
			piece.respawn(bag.popNextPiece());
			// TODO: scoring
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::R)) {
			// Restart
			inputHandler.addToCooldown(sf::Keyboard::Scan::R);
			board.reset();
			// TODO: also reset bag, piece, etc.
		}

		// TODO: more updates (falling)
		// Render
		renderer.renderGame(board, piece, bag);
	}
	return 0;
}
