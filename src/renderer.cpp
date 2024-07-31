#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window, AssetHandler& assetHandler) : window(window), assetHandler(assetHandler) {
	// TODO: update defaults if needed
	// TODO: allow config
	blockWidth = 30;
}

void Renderer::renderRect(int x, int y, int width, int height, Color color, int opacity = 255) {
	sf::RectangleShape toRender;
	toRender.setSize(sf::Vector2f(width, height));
	toRender.setPosition(sf::Vector2f(x, y));
	toRender.setFillColor(sf::Color(color.r, color.g, color.b, opacity));
	window.draw(toRender);
}

void Renderer::renderText(int x, int y, int size, Color color, sf::Font& font, std::string str) {
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setFillColor(sf::Color(color.r, color.g, color.b));
	text.setPosition(sf::Vector2f(x, y));
	window.draw(text);
}

void Renderer::renderBlock(int x, int y, int boardOffsetx, int boardOffsety, int widthToUse, Block cellHere, int sparkleValue = 0) {
	Color mainColor = blockToColor(cellHere);
	Color postEffectsColor = lerpWhiteColor(mainColor, (100 - sparkleValue) / 100.0);
	// Main block outside
	renderRect(boardOffsetx + x * widthToUse, boardOffsety + y * widthToUse, widthToUse, widthToUse, multColor(postEffectsColor, 0.9));
	// Main block inside
	renderRect(boardOffsetx + x * widthToUse + widthToUse / 10, boardOffsety + y * widthToUse + widthToUse / 10, widthToUse * 8 / 10, widthToUse * 8 / 10, postEffectsColor);
	// Top highlight (if needed)
	if (cellHere != Block::Shadow) {
		renderRect(boardOffsetx + x * widthToUse + widthToUse / 10, boardOffsety + y * widthToUse + widthToUse / 10, widthToUse * 8 / 10, widthToUse / 10, lerpWhiteColor(postEffectsColor, 0.8));
	}
}

void Renderer::renderGame(Board& board, Piece& piece, Bag& bag, Block holdBlock, Score& score, Effects& effects) {
	// Calculate the shadow piece
	Piece shadow(piece);
	for (int i = 0; i < 20; i++) {
		shadow.move(0, 1, board);
	}
	// Clear
	window.clear(sf::Color(19, 22, 36));
	// Render the board
	// TODO: special graphics effects (static 'noise', some cells should be slightly darker than others, darker cell "gradients"/borders rendered via colors rather than images, non-intrusive line clearing fx, glitter/shine, etc.)
	int boardOffsety = blockWidth * 2;
	for (int y = 0; y < board.getHeight(); y++) {
		for (int x = 0; x < board.getWidth(); x++) {
			// Render a rectangle, if desired
			// TODO: more effects
			// Determine whether to render the piece or the existing board cell
			Block pieceCellHere = piece.getCell(x - piece.getLocx(), y - piece.getLocy());
			if (score.getDead() && pieceCellHere != Block::None) pieceCellHere = Block::Dead;
			Block boardCellHere = board.getCell(x, y);
			Block shadowCellHere = shadow.getCell(x - shadow.getLocx(), y - shadow.getLocy());
			if (pieceCellHere != Block::None) {
				// Render the piece cell
				renderBlock(x, y, 0, boardOffsety, blockWidth, pieceCellHere, effects.getSparkleValue(x, y));
			} else if (boardCellHere != Block::None) {
				// Render the existing board cell
				renderBlock(x, y, 0, boardOffsety, blockWidth, boardCellHere, effects.getSparkleValue(x, y));
			} else if (shadowCellHere != Block::None) {
				// Render the shadow cell
				renderBlock(x, y, 0, boardOffsety, blockWidth, Block::Shadow);
			} else {
				// Empty: render the grid
				renderRect(x * blockWidth + blockWidth / 10, boardOffsety + y * blockWidth + blockWidth / 10, blockWidth * 8 / 10, blockWidth * 8 / 10, { 22, 25, 40 });
				// Empty: render the edges of pieces, if needed
				// TODO: setting to enable/disable these effects
				if (RENDER_EDGES) {
					if (board.getCell(x - 1, y + 1) != Block::None) {
						// Bottom left corner (render first to get overlapped if needed)
						renderRect(x * blockWidth, boardOffsety + y * blockWidth + 9 * blockWidth / 10, blockWidth / 10, blockWidth / 10, multColor(blockToColor(board.getCell(x - 1, y + 1)), 0.7));
					}
					if (board.getCell(x - 1, y) != Block::None) {
						// Left edge
						renderRect(x * blockWidth, boardOffsety + y * blockWidth, blockWidth / 10, blockWidth, multColor(blockToColor(board.getCell(x - 1, y)), 0.7));
					}
					if (board.getCell(x, y + 1) != Block::None) {
						// Left edge
						renderRect(x * blockWidth, boardOffsety + y * blockWidth + 9 * blockWidth / 10, blockWidth, blockWidth / 10, multColor(blockToColor(board.getCell(x, y + 1)), 0.7));
					}
				}
			}
			// Effects
			// Beam
			if (y == effects.getBeamLocy()) {
				renderRect(0, boardOffsety + y * blockWidth + 9 * blockWidth / 10, blockWidth * board.getWidth(), blockWidth / 10, { 255, 255, 255 }, effects.getBeamStrength() * 255 / 100);
			}
		}
	}
	// Flash the screen if needed
	if (effects.getFlashStrength() > 0) {
		// Flash
		renderRect(0, boardOffsety, blockWidth * board.getWidth(), blockWidth * board.getHeight(), effects.getFlashColor(), effects.getFlashStrength() * 255 / 100);
	}
	// Show the queue
	// TODO: queue size config
	std::vector<Block> futurequeue = bag.viewFuture(5);
	int miniWidth = blockWidth / 2;
	int blockDisplayStartx = board.getWidth() * blockWidth + miniWidth;
	int blockDisplayStarty = miniWidth;
	for (int i = 0; i < futurequeue.size(); i++) {
		Block blockHere = futurequeue[i];
		// Display the block (smaller than usual, on the right side)
		for (int y = 0; y < PIECE_SIZE; y++) {
			for (int x = 0; x < PIECE_SIZE; x++) {
				Block cellHere = numberToBlock(PIECE_DATA[blockToIndex(blockHere)][0][y][x]);
				if (cellHere != Block::None) {
					renderBlock(x, y, blockDisplayStartx, blockDisplayStarty + i * 4 * miniWidth, miniWidth, cellHere);
				}
			}
		}
	}
	// Show the hold block
	for (int y = 0; y < PIECE_SIZE; y++) {
		for (int x = 0; x < PIECE_SIZE; x++) {
			Block cellHere = numberToBlock(PIECE_DATA[blockToIndex(holdBlock)][0][y][x]);
			if (cellHere != Block::None) {
				renderBlock(x, y, miniWidth, boardOffsety + miniWidth, miniWidth, cellHere);
			}
		}
	}
	// TODO: More animations for clearing lines, chains/combos/b2bs, etc
	// TODO: UI: Show controls info
	sf::Font& font = assetHandler.getFont();
	renderText(
		board.getWidth() * blockWidth, board.getHeight() * blockWidth - 6 * blockWidth, 18,
		{ 77, 84, 112 }, font, "[R] Restart"
	);
	// Other color: { 146, 152, 176 }
	// UI: Show current score data
	// TODO: improve appearance, show towards top of screen
	std::string scoreString1 = "";
	scoreString1 += "Score: ";
	scoreString1 += std::to_string(score.points);
	scoreString1 += "\nLines: ";
	scoreString1 += std::to_string(score.lines);
	std::string scoreString2 = "";
	scoreString2 += "Time: ";
	scoreString2 += std::to_string((int) (score.timeElapsed / 1000));
	if (score.currStreak > 0) {
		scoreString2 += "\nB2B: ";
		scoreString2 += std::to_string(score.currStreak);
	}
	//renderText(board.getWidth() * blockWidth, board.getHeight() * blockWidth - 4 * blockWidth, 18, { 77, 84, 112 }, font, scoreString);
	renderText(blockWidth, 10, 18, { 145, 151, 179 }, font, scoreString1);
	renderText(blockWidth * 5, 10, 18, { 145, 151, 179 }, font, scoreString2);
	// Display
	window.display();
}

void Renderer::renderMenu(AssetHandler& assetHandler, Menu menu, Config& config) {
	// TODO: impl better
	// Prepare to render UI
	sf::Font& font = assetHandler.getFont();
	// UI: Show info based on the menu
	switch (menu) {
	case Menu::Title:
		// Title screen
		window.clear(sf::Color(19, 22, 36));
		// TODO: for title screen, display the title of the game and a demo board (create a whole fake game for the renderGame func?)
		// TODO: moving title animation with a sine wave based on milliseconds elapsed
		renderText(DEFAULT_WINDOW_WIDTH / 2 - 100 + (int) (sin(currentTimeMs() / 3000.0) * 10), 200 + (int) (sin(currentTimeMs() / 1000.0) * 20), 32, { 219, 70 + (int) (sin(currentTimeMs() / 2000.0) * 40), 24 }, font, "~ SUPERTRIS ~");
		renderText(30, window.getSize().y / 2 - 10, 18, { 145, 151, 179 }, font, "[1] Start\n[2] Config\n[3] Leaderboard\n\n[Esc]/[Q] Quit");
		break;
	case Menu::Config: {
		// Config screen
		// TODO: allow config options
		window.clear(sf::Color(19, 22, 36));
		renderText(DEFAULT_WINDOW_WIDTH / 2 - 100 + (int) (sin(currentTimeMs() / 3000.0) * 10), 200 + (int) (sin(currentTimeMs() / 1000.0) * 20), 32, { 219, 70 + (int) (sin(currentTimeMs() / 2000.0) * 40), 24 }, font, "~ SUPERTRIS ~");
		std::string toDisplay = "[1] Control Mode\n";
		toDisplay += "[2] ARR = " + std::to_string(config.getArr()) + "\n";
		toDisplay += "[3] DAS = " + std::to_string(config.getDas()) + "\n";
		toDisplay += "[4] SDF = " + std::to_string(config.getSdf()) + "\n";
		toDisplay += "\n[Esc] Back to menu";
		renderText(30, window.getSize().y / 2 - 10, 18, { 145, 151, 179 }, font, toDisplay);
		break;
	}
	case Menu::Leaderboard:
		// Leaderboard screen
		// TODO: allow clearing
		// TODO: impl highscores
		// TODO: impl display of highscores
		window.clear(sf::Color(19, 22, 36));
		renderText(DEFAULT_WINDOW_WIDTH / 2 - 100 + (int) (sin(currentTimeMs() / 3000.0) * 10), 200 + (int) (sin(currentTimeMs() / 1000.0) * 20), 32, { 219, 70 + (int) (sin(currentTimeMs() / 2000.0) * 40), 24 }, font, "~ SUPERTRIS ~");
		renderText(30, window.getSize().y / 2 - 10, 18, { 145, 151, 179 }, font,
			"Normal mode:\n  1. --EMPTY--\n  2. --EMPTY--\n  3. --EMPTY--\n  4. --EMPTY--\n\n[Esc] Back to menu"
		);
		break;
	case Menu::Dead:
		// Dead: clear the bottom right part of the screen only
		window.display();
		int menuwidth = 100;
		int menuheight = 300;
		int menux = window.getSize().x - menuwidth;
		int menuy = window.getSize().y - menuheight;
		renderRect(menux, menuy, menuwidth, menuheight, { 19, 22, 36 });
		renderText(menux, menuy + 10, 18, { 145, 151, 179 }, font, "Game over!\n[R] Restart\n\n[Esc] Menu");
		break;
	}
	// TODO: bottom tooltip
	// Display
	window.display();
	// TODO: display twice to show the actual gray death screen, in the case of being dead
}
