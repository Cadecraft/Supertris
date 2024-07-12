#include <SFML/Graphics.hpp>
#include <iostream>

#include "constants.h"
#include "utils.h"
#include "board.h"
#include "piece.h"
#include "renderer.h"
#include "bag.h"
#include "config.h"
#include "inputhandler.h"
#include "assethandler.h"

/* TODO:
	- does build work on Linux or Mac?
	- implement the whole game: clearing lines, advanced score and points (ex. quads and continued), tspins
	- when run from command line, sometimes fails to find the font asset?
	- smarter rotation checking/locking (from guideline?)
	- death and dead-colored blocks
	- arr/das/sdf settings
	- graphics improve
	- UI system with config, game modes, etc.
	- click-based button UI as well as keyboard-based number UI
	- other?
*/

// Entry point
int main() {
	// TODO: save window position, allow resizing in the menu
	auto window = sf::RenderWindow{ { DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT }, "Supertris" };
	// TODO: better framerate limit ?
	window.setFramerateLimit(144);

	// Create game objects
	Board board;
	Bag bag;
	Piece piece(bag.popNextPiece());
	InputHandler inputHandler;
	AssetHandler assetHandler;
	Renderer renderer(window, assetHandler);
	Score score(currentTimeMs());
	Block holdBlock = Block::None;
	Config config;
	Effects effects;
	effects.resetByBoard(board); // TODO: refactor?
	bool canSwapHold = true; // TODO: refactor better for when piece locks, update when game is reset
	uint64_t fallTimerTarget = currentTimeMs() + score.getMsPerFall(); // TODO: refactor into various timers?
	bool prevAutodropShouldLock = false; // TODO: refactor into various timers/data
	uint64_t autodropTimerTarget = currentTimeMs() + score.getMsPerAutolock();
	// Timing
	// TODO: timing fix (test calculation of ms elapsed) !
	uint64_t previousTime = currentTimeMs();
	uint64_t startTime = currentTimeMs();

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
		score.updateTime(currentTimeMs());
		// Falling
		if (currentTimeMs() > fallTimerTarget) {
			fallTimerTarget = currentTimeMs() + score.getMsPerFall();
			// Fall
			bool shouldLock = piece.move(0, 1, board);
			shouldLock = piece.shouldLockNext(board);
			// TODO: only lock after sitting there for 1000ms
			// TODO: separate timer (store in the piece class or something?)?
			// TODO: FIX !
			if (shouldLock) {
				if (prevAutodropShouldLock && currentTimeMs() > autodropTimerTarget) {
					prevAutodropShouldLock = false;
					// Lock and reset
					int cleared = board.lockPiece(piece);
					score.increase(cleared, false, board.isClear()); // TODO: handle tspins
					if (cleared == 0) effects.sparklePiece(piece);
					else if (cleared == 4) effects.spawnFlash(COLOR_LINECLEAR, 40); // TODO: also flash on tspin
					//else effects.spawnBeam(piece.getLocy()); // TODO: fix to use actual line clear bottom
					piece.respawn(bag.popNextPiece()); // TODO: refactor the lock process
					canSwapHold = true;
					if (!piece.isValid(board)) {
						// TODO: refactor this too
						board.die();
						score.die();
					}
					autodropTimerTarget = currentTimeMs() + score.getMsPerAutolock(); // Re-update timer
				} else {
					// Do not lock; set the timer
					prevAutodropShouldLock = true;
					autodropTimerTarget = currentTimeMs() + score.getMsPerAutolock(); // TODO: incr based on ms per fall AND this
					fallTimerTarget = autodropTimerTarget - 1;
				}
			} else {
				prevAutodropShouldLock = false;
			}
		}
		// Update input
		inputHandler.updateCooldowns(msElapsed);
		// Process input
		if (inputHandler.isActive(sf::Keyboard::Scan::A) && !inputHandler.inCooldownData(sf::Keyboard::Scan::D)) {
			// Left
			if (inputHandler.inCooldownData(sf::Keyboard::Scan::A)) {
				inputHandler.addToCooldown(sf::Keyboard::Scan::A, config.getArr());
				if (config.getArr() == 0) {
					for (int i = 0; i < 10; i++) {
						piece.move(-1, 0, board);
					}
				}
			} else {
				inputHandler.addToCooldown(sf::Keyboard::Scan::A, config.getDas());
			}
			piece.move(-1, 0, board);
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::D) && !inputHandler.inCooldownData(sf::Keyboard::Scan::A)) {
			// Right
			if (inputHandler.inCooldownData(sf::Keyboard::Scan::D)) {
				inputHandler.addToCooldown(sf::Keyboard::Scan::D, config.getArr());
				if (config.getArr() == 0) {
					for (int i = 0; i < 9; i++) {
						piece.move(1, 0, board);
					}
				}
			} else {
				inputHandler.addToCooldown(sf::Keyboard::Scan::D, config.getDas());
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
			inputHandler.addToCooldown(sf::Keyboard::Scan::W, config.getSdf()); // TODO: refactor into SDF
			bool shouldLock = piece.move(0, 1, board);
			if (config.getSdf() == 0) {
				for (int i = 0; i < 19; i++) {
					shouldLock = piece.move(0, 1, board);
				}
			}
			shouldLock = false; // TODO: only lock if enough time has elapsed since the piece first hit the bottom
			// TODO: non-locking for a cooldown second if staying in the same spot
			if (shouldLock) {
				// Lock and reset
				int cleared = board.lockPiece(piece);
				score.increase(cleared, false, board.isClear()); // TODO: handle tspins
				if (cleared == 0) effects.sparklePiece(piece);
				else if (cleared == 4) effects.spawnFlash(COLOR_LINECLEAR, 40); // TODO: also flash on tspin
				//else effects.spawnBeam(piece.getLocy()); // TODO: fix to use actual line clear bottom
				piece.respawn(bag.popNextPiece());
				canSwapHold = true;
				if (!piece.isValid(board)) {
					// TODO: refactor this too
					board.die();
					score.die();
				}
				autodropTimerTarget = currentTimeMs() + score.getMsPerAutolock(); // Re-update timer
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
			score.increase(cleared, false, board.isClear()); // TODO: handle tspins
			if (cleared == 0) effects.sparklePiece(piece);
			else if (cleared == 4) effects.spawnFlash(COLOR_LINECLEAR, 40); // TODO: also flash on tspin
			//else effects.spawnBeam(piece.getLocy()); // TODO: fix to use actual line clear bottom
			piece.respawn(bag.popNextPiece());
			canSwapHold = true;
			if (!piece.isValid(board)) {
				// TODO: refactor this too
				board.die();
				score.die();
			}
			autodropTimerTarget = currentTimeMs() + score.getMsPerAutolock(); // Re-update timer
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::R)) {
			// Restart
			inputHandler.addToCooldown(sf::Keyboard::Scan::R);
			board.reset();
			// TODO: also reset bag, piece, score, etc. (maybe just return from a new "game" function and then continue the loop?)
			score.reset(currentTimeMs());
			// TODO: also reset piece
		}
		if (inputHandler.isActive(sf::Keyboard::Scan::LShift) || inputHandler.isActive(sf::Keyboard::Scan::RShift)) {
			// Swap the current hold piece, if possible
			inputHandler.addToCooldown(sf::Keyboard::Scan::LShift);
			inputHandler.addToCooldown(sf::Keyboard::Scan::RShift);
			if (canSwapHold) {
				canSwapHold = false;
				Block swapTo = holdBlock;
				if (swapTo == Block::None) swapTo = bag.popNextPiece();
				holdBlock = piece.getPieceType();
				piece.respawn(swapTo);
			}
		}

		// Update graphics
		effects.updateByFrame();
		// Render
		renderer.renderGame(board, piece, bag, holdBlock, score, effects);
	}
	return 0;
}
