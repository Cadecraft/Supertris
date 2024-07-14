#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

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

int executeMenu(sf::RenderWindow& window, Menu menu);
GameReturnType executeGame(sf::RenderWindow& window);

// Entry point
int main() {
	// TODO: save window position, allow resizing in the menu
	auto window = sf::RenderWindow{ { DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT }, "Supertris" };
	// TODO: better framerate limit ?
	window.setFramerateLimit(144);
	std::srand(time(NULL));

	// Start the system loop
    // Initial menu execution
    executeMenu(window, Menu::Title);
	/*while (window.isOpen()) {
		// TODO: better menus and other things
		// Execute a round of the game
		GameReturnType returned = executeGame(window);
		switch (returned) {
		case GameReturnType::Restart:
			// Restart immediately
			break;
		case GameReturnType::Die:
			// Update, render text and await input
			// TODO: highscore updates, display "game over", etc.
			executeMenu(window, Menu::Dead);
			break;
		case GameReturnType::WindowClose:
			// Closed the window
			break;
		}
	}*/
	return 0;
}

// Run the game and handle all menus
int runGame(sf::RenderWindow& window) {
    // TODO: better menus and other things
    // Execute a round of the game
    GameReturnType returned = executeGame(window);
    switch (returned) {
        case GameReturnType::Restart:
            // Restart immediately
            runGame(window);
            break;
        case GameReturnType::Die:
            // Update, render text and await input
            // TODO: highscore updates, display "game over", etc.
            executeMenu(window, Menu::Dead);
            break;
        case GameReturnType::WindowClose:
            // Closed the window
            break;
    }
    return 0;
}

// Collect user input in the menu
int executeMenu(sf::RenderWindow& window, Menu menu) {
	// Create menu objects
	InputHandler inputHandler;
	AssetHandler assetHandler;
	Renderer renderer(window, assetHandler);

	// TODO: render the menu well, including the original blocks from the prior game??
    // TODO: menu navigation, sort of recursively calling with different menus or returning
	renderer.renderMenu(assetHandler, menu);

	// Main input loop
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
		// Process input based on the menu
        switch (menu) {
        case Menu::Title:
            if (inputHandler.isActive(sf::Keyboard::Scan::Q) || inputHandler.isActive(sf::Keyboard::Scan::Escape)) {
                inputHandler.addToCooldown(sf::Keyboard::Scan::Q);
                inputHandler.addToCooldown(sf::Keyboard::Scan::Escape);
                // Quit
                window.close();
                return 0;
            } else if (inputHandler.isActive(sf::Keyboard::Scan::Num1)) {
                inputHandler.addToCooldown(sf::Keyboard::Scan::Num1);
                // Play
                runGame(window);
            } else if (inputHandler.isActive(sf::Keyboard::Scan::Num2)) {
                inputHandler.addToCooldown(sf::Keyboard::Scan::Num2);
                // Config
                executeMenu(window, Menu::Config);
            }
            break;
        case Menu::Config:
            if (inputHandler.isActive(sf::Keyboard::Scan::Q) || inputHandler.isActive(sf::Keyboard::Scan::Escape)) {
                inputHandler.addToCooldown(sf::Keyboard::Scan::Q);
                inputHandler.addToCooldown(sf::Keyboard::Scan::Escape);
                // Back to main menu
                executeMenu(window, Menu::Title);
            } else if (inputHandler.isActive(sf::Keyboard::Scan::Num1)) {
                inputHandler.addToCooldown(sf::Keyboard::Scan::Num1);
                // Edit the config
                // TODO: impl
            }
            break;
        case Menu::Dead:
            if (inputHandler.isActive(sf::Keyboard::Scan::Q) || inputHandler.isActive(sf::Keyboard::Scan::Escape)) {
                inputHandler.addToCooldown(sf::Keyboard::Scan::Q);
                inputHandler.addToCooldown(sf::Keyboard::Scan::Escape);
                // Back to main menu
                executeMenu(window, Menu::Title);
            } else if (inputHandler.isActive(sf::Keyboard::Scan::R)) {
                inputHandler.addToCooldown(sf::Keyboard::Scan::R);
                // Restart
                runGame(window);
            }
            break;
        }
		// TODO: render?
	}
	return 0;
}

// Execute a round of the game
GameReturnType executeGame(sf::RenderWindow& window) {
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
			inputHandler.addToCooldown(sf::Keyboard::Scan::W, config.getSdf());
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
			// TODO: why does the bag not fully reset sometimes?
			// Return from the function to completely reset the game (system loop is above, in the main function)
			return GameReturnType::Restart;
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

		// If dead, return
		if (score.getDead()) return GameReturnType::Die;
	}
	return GameReturnType::WindowClose;
}
